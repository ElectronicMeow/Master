//
// Created by reverier on 2021/10/18.
//

#include "logger.h"

#include <time.h>
#include <string.h>

void log_head_(const char *head, int color, const char *level);

void log_debug(const char *head, const char *fmt, ...) {
    log_head_(head, 42, "DEBUG");
    va_list arg_list;
    va_start(arg_list, fmt);
    vprintf(fmt, arg_list);
    va_end(arg_list);
    putchar('\n');
}

void log_info(const char *head, const char *fmt, ...) {
    log_head_(head, 44, "INFO");
    va_list arg_list;
    va_start(arg_list, fmt);
    vprintf(fmt, arg_list);
    va_end(arg_list);
    putchar('\n');
}

void log_warn(const char *head, const char *fmt, ...) {
    log_head_(head, 43, "WARN");
    va_list arg_list;
    va_start(arg_list, fmt);
    vprintf(fmt, arg_list);
    va_end(arg_list);
    putchar('\n');
}

void log_error(const char *head, const char *fmt, ...) {
    log_head_(head, 41, "ERROR");
    va_list arg_list;
    va_start(arg_list, fmt);
    vprintf(fmt, arg_list);
    va_end(arg_list);
    putchar('\n');
}

void log_head_(const char *head, int color, const char *level) {
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    char *time_str = asctime(time_info);
    time_str[strlen(time_str) - 1] = 0;
    printf("%s \x1B[97;%dm %s \x1B[0m [ %s ] ",
           time_str,
           color,
           level,
           head);
}
