//
// Created by Reverier-Xu on 2021/10/10.
//

#include "http/mongoose.h"
#include "initialization/initialization.h"

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                                        // Init manager
    init_router(&mgr);
    while(1)
        mg_mgr_poll(&mgr, 1000);                         // noqa Event loop
    mg_mgr_free(&mgr);                                        // Cleanup
    return 0;
}