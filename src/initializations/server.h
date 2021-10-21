//
// Created by reverier on 2021/10/21.
//

#ifndef MEOWMASTER_SERVER_H
#define MEOWMASTER_SERVER_H

#include "http/httpServer.h"
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include "services/handler.h"
#include "utils/crypto_utils.h"


HttpServer *initServer() {
    const QStringList CONFIG_FILE_PATH = {
            "~/.config/",
            "/etc/",
            "/opt/",
            "./",
    };
    QString config_file;
    bool ok = false;
    for (auto &i: CONFIG_FILE_PATH) {
        config_file = i + "meow_master.conf";
        if (QFile::exists(config_file)) {
            ok = true;
            break;
        }
    }

    if (!ok) {
        qWarning("Could not found server configurations!");
        qWarning("Server Address default to 127.0.0.1:7200.");
    }

    QSettings settings(config_file, QSettings::IniFormat);
    settings.beginGroup("Server");
    auto address = settings.value("Address", "127.0.0.1").toString();
    auto port = settings.value("Port", 7200).toInt();
    settings.endGroup();
    settings.beginGroup("Spec");

    QString spec_P(settings.value("P", "0").toString());
    QString spec_Q(settings.value("Q", "0").toString());

    if (spec_P == "0" or spec_Q == "0") {
        qWarning(
                "No specs found in Master's configuration file! using random generated specs, it may takes a long time...");
        MeowCryptoUtils::specGen(spec_P, spec_Q);
        qInfo("Spec generated.");
        qInfo("spec_P = %s", spec_P.toStdString().c_str());
        qInfo("spec_Q = %s", spec_Q.toStdString().c_str());
    }
    settings.endGroup();

    settings.beginGroup("Connection");

    auto ccAddress = settings.value("ComputingCenter").toString();

    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(ccAddress + "/register-master");
    req.setRawHeader("Authorization", "Bearer SAMPLE-TOKEN"); // TODO: changes it.
    manager.post(req,
                 QString(R"({"data":{"url":"%1"}})").arg(address + ":" + QString::number(port)).toLocal8Bit());

    settings.endGroup();

    auto server_config = HttpServerConfig();
    server_config.host = QHostAddress(address);
    server_config.port = port;
    server_config.verbosity = HttpServerConfig::Verbose::Info;

    auto handler = new MainHandler();
    handler->setSpecs(spec_P, spec_Q);

    auto server = new HttpServer(server_config, handler);
    qInfo("Server is running on %s:%d !", address.toStdString().c_str(), port);

    return server;
}

#endif //MEOWMASTER_SERVER_H
