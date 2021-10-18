//
// Created by Reverier-Xu on 2021/10/10.
//

#include "http/mongoose.h"
#include "initialization/initialization.h"
#include "logger/logger.h"

int main(int argc, char *argv[]) {
    log_info("entry", "-*- Meow Master Server Started -*-");
    log_info("entry", "Welcome to Electron Meow Project!");
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                                        // Init manager
    init_router(&mgr, "0.0.0.0:8000");

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1)
        mg_mgr_poll(&mgr, 1000);                         // noqa Event loop
#pragma clang diagnostic pop

    mg_mgr_free(&mgr);                                        // Cleanup
    return 0;
}