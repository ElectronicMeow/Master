//
// Created by reverier on 2021/10/21.
//

#include <QJsonArray>
#include "handler.h"
#include "utils/crypto_utils.h"


MainHandler::MainHandler() {
    this->router_.addRoute("GET", "^/generate-keys$", this, &MainHandler::handleGenerateKeys);
    this->router_.addRoute("POST", "^/transform-to-same-pk$", this, &MainHandler::handleTransformToSamePk);
    this->router_.addRoute("POST", "^/transform-back$", this, &MainHandler::handleTransformBack);
}

void MainHandler::setSpecs(const QString &specP, const QString &specQ) {
    this->specP_ = specP;
    this->specQ_ = specQ;
}

HttpPromise MainHandler::handle(HttpDataPtr data) {
    bool foundRoute;
    HttpPromise promise = this->router_.route(data, &foundRoute);
    if (foundRoute)
        return promise;

    qWarning("Request URI in this method is not found.");

    QJsonObject object;
    object["message"] = "INVALID URI or Method.";

    data->response->setStatus(HttpStatus::Forbidden, QJsonDocument(object));
    return HttpPromise::resolve(data);
}

HttpPromise MainHandler::handleGenerateKeys(HttpDataPtr data) {
    bool ok = MeowCryptoUtils::keyGenMaster(this->specP_, this->specQ_, this->publicParameters_, this->sk_);

    if (!ok) {
        data->response->setStatus(HttpStatus::InternalServerError,
                                  QJsonDocument::fromJson(R"({"message":"could not gen keys."})"));
        return HttpPromise::resolve(data);
    }

    QJsonObject res;
    res["N"] = this->publicParameters_.N();
    res["k"] = this->publicParameters_.k();
    res["g"] = this->publicParameters_.g();

    qInfo("Master SK and PublicParams generated: N=%s, k=%s, g=%s", this->publicParameters_.N().toStdString().c_str(),
          this->publicParameters_.k().toStdString().c_str(), this->publicParameters_.g().toStdString().c_str());
    qInfo("SecretKey is: p=%s, q=%s", this->sk_.A().toStdString().c_str(), this->sk_.B().toStdString().c_str());

    QJsonObject resp;
    resp["data"] = res;
    resp["message"] = "ok";

    data->response->setStatus(HttpStatus::Ok, QJsonDocument(resp));

    return HttpPromise::resolve(data);
}

HttpPromise MainHandler::handleTransformToSamePk(HttpDataPtr data) {
    if (data->request->mimeType().compare("application/json", Qt::CaseInsensitive) != 0) {
        qWarning("Incorrect data type in handleTransformToSamePk.");
        throw HttpException(HttpStatus::BadRequest, "Request body content type must be application/json");
    }
    QJsonDocument jsonDocument = data->request->parseJsonBody();
    if (jsonDocument.isNull()) {
        qWarning("Incorrect data format in handleTransformToSamePk.");
        throw HttpException(HttpStatus::BadRequest, "Invalid JSON body");
    }

    QJsonArray res;
    QList<MeowCryptoUtils::EncryptedPair> cipher_list;

    try {
        auto ciphers = jsonDocument.object()["data"].toArray();
        for (auto cipher: ciphers) {
            auto obj = cipher.toArray();
            auto tag = obj[0].toString();
            MeowCryptoUtils::EncryptedPair c;
            c.setA(obj[1].toString());
            c.setB(obj[2].toString());
            c.setPublicKey(obj[3].toString());
            c.setPublicN(this->publicParameters_.N());
            this->pk_reverse_map_[c.publicKey()] = tag;
            cipher_list.append(c);
        }
    } catch (std::exception &e) {
        throw HttpException(HttpStatus::BadRequest, "Bad data");
    }

    auto prod_pk = MeowCryptoUtils::getProdKey(this->publicParameters_, this->pk_reverse_map_.keys());
    this->prod_pk_ = prod_pk;
    MeowCryptoUtils::EncryptedPair r;
    for (auto &i : cipher_list) {
        r = MeowCryptoUtils::masterTransform(this->publicParameters_, this->sk_, prod_pk, i);
        QJsonArray record;
        record.append(this->pk_reverse_map_[i.publicKey()]); // tag
        record.append(r.A()); // encrypted pair A
        record.append(r.B()); // encrypted pair B
        res.append(record);
    }

    QJsonObject resp;
    resp["data"] = res;
    resp["message"] = "ok";

    data->response->setStatus(HttpStatus::Ok, QJsonDocument(resp));

    return HttpPromise::resolve(data);
}

HttpPromise MainHandler::handleTransformBack(HttpDataPtr data) {
    if (data->request->mimeType().compare("application/json", Qt::CaseInsensitive) != 0) {
        qWarning("Incorrect data type in handleTransformToSamePk.");
        throw HttpException(HttpStatus::BadRequest, "Request body content type must be application/json");
    }
    QJsonDocument jsonDocument = data->request->parseJsonBody();
    if (jsonDocument.isNull()) {
        qWarning("Incorrect data format in handleTransformToSamePk.");
        throw HttpException(HttpStatus::BadRequest, "Invalid JSON body");
    }

    MeowCryptoUtils::EncryptedPair inp;
    inp.setPublicKey(this->prod_pk_);
    inp.setPublicN(this->publicParameters_.N());
    inp.setA(jsonDocument.object()["encrypted_pair_a"].toString());
    inp.setB(jsonDocument.object()["encrypted_pair_b"].toString());

    QJsonArray ans;

    for (auto &pk : this->pk_reverse_map_.keys()) {
        auto tag = this->pk_reverse_map_[pk];
        auto r = MeowCryptoUtils::masterTransform(this->publicParameters_, this->sk_, pk, inp);

        QJsonArray record;
        record.append(tag); // tag
        record.append(r.A()); // encrypted pair A
        record.append(r.B()); // encrypted pair B
        ans.append(record);
    }

    QJsonObject resp;
    resp["data"] = ans;
    resp["message"] = "ok";

    data->response->setStatus(HttpStatus::Ok, QJsonDocument(resp));

    return HttpPromise::resolve(data);
}
