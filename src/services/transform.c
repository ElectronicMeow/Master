//
// Created by Reverier-Xu on 2021/10/13.
//

#include "transform.h"

#include "http/mjson.h"

int transform_to_same_pk(char ** resp) {
    *resp = mjson_aprintf("{%Q:%Q}", "message", "ok");
    return 1;
}

int transform_back(char ** resp) {
    *resp = mjson_aprintf("{%Q:%Q}", "message", "ok");
    return 1;
}