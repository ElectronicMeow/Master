//
// Created by reverier on 2021/10/20.
//

#ifndef MEOWMASTER_LOGGER_H
#define MEOWMASTER_LOGGER_H

#include <QMessageLogger>


void initLogger() {
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss]} - "
                       "%{if-debug}\033[97;43m DBG%{endif}"
                       "%{if-info}\033[97;44m INF%{endif}"
                       "%{if-warning}\033[97;43m WRN%{endif}"
                       "%{if-critical}\033[97;45m CRT%{endif}"
                       "%{if-fatal}\033[97;41m ERR%{endif} \033[0m %{function}: %{message}");
}

#endif //MEOWMASTER_LOGGER_H
