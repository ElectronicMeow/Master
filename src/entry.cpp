//
// Created by Reverier-Xu on 2021/10/19.
//

#include <QCoreApplication>
#include <QMessageLogger>
#include "initializations/logger.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    initLogger();

//  qDebug("This is a debug message!");
    qInfo("Welcome to Electronic Meow Project!");
    qWarning("This Server is Master Key Server; Booting Server...");

    /// server initializations

    return QCoreApplication::exec();
}
