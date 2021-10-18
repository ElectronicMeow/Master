//
// Created by Reverier-Xu on 2021/10/18.
//

#ifndef MEOWMASTER_LOGGER_H
#define MEOWMASTER_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void log_debug(const char *head, const char *fmt, ...);

void log_info(const char *head, const char *fmt, ...);

void log_warn(const char *head, const char *fmt, ...);

void log_error(const char *head, const char *fmt, ...);

#endif //MEOWMASTER_LOGGER_H
