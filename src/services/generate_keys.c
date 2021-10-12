//
// Created by Reverier-Xu on 2021/10/13.
//

#include "generate_keys.h"

#include <http/mjson.h>

int generate_keys(char ** resp) {
    *resp = mjson_aprintf("{%Q:%Q}", "message", "ok");
    return 1;
}
