//
// Created by Reverier-Xu on 2021/10/13.
//

#ifndef MEOWMASTER_INITIALIZATION_H
#define MEOWMASTER_INITIALIZATION_H

#include "http/mongoose.h"

int init_router(struct mg_mgr* mgr, const char* addr);

#endif //MEOWMASTER_INITIALIZATION_H
