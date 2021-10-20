//
// Created by Reverier-Xu on 2021/10/19.
//

#include <QCoreApplication>
#include <QMessageLogger>
#include <QDebug>


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss]} - "
                       "%{if-debug}\033[97;43m DBG%{endif}"
                       "%{if-info}\033[97;44m INF%{endif}"
                       "%{if-warning}\033[97;43m WRN%{endif}"
                       "%{if-critical}\033[97;45m CRT%{endif}"
                       "%{if-fatal}\033[97;41m ERR%{endif} \033[0m %{function}: %{message}");
    qDebug("This is a debug message!");
    qInfo("Welcome to Electronic Meow Project!");
    qWarning("Booting now...");
    qCritical("This is a critical message.");
    qFatal("This is a fatal message.");

    return QCoreApplication::exec();
}
