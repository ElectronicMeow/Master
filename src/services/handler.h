//
// Created by reverier on 2021/10/21.
//

#ifndef MEOWMASTER_HANDLER_H
#define MEOWMASTER_HANDLER_H

#include "http/httpRequestRouter.h"
#include "http/httpRequestHandler.h"
#include "utils/crypto_utils.h"

class MainHandler : public HttpRequestHandler {
private:
    HttpRequestRouter router_;
    QString specP_;
    QString specQ_;
    MeowCryptoUtils::MasterSecretKeyPair sk_;
    MeowCryptoUtils::PublicParameters publicParameters_;
    QMap<QString, QString> pk_reverse_map_;
    QString prod_pk_; // just for storage.

public:
    MainHandler();

    void setSpecs(const QString& specP, const QString& specQ);

    HttpPromise handle(HttpDataPtr data) override;

    HttpPromise handleGenerateKeys(HttpDataPtr data);

    HttpPromise handleTransformToSamePk(HttpDataPtr data);

    HttpPromise handleTransformBack(HttpDataPtr data);
};


#endif //MEOWMASTER_HANDLER_H
