//
// Created by Reverier-Xu on 2021/10/13.
//

#include "initialization.h"
#include "http/mjson.h"

#include "services/generate_keys.h"
#include "services/transform.h"
#include "logger/logger.h"

void init_router_(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

/**
 * 初始化服务器api
 * @param mgr 服务器 manager。
 * @return 是否初始化成功，成功返回 1，不成功返回 0。
 */
int init_router(struct mg_mgr *mgr, const char* addr) {
    log_info("init", "listening on %s...", addr);
    mg_http_listen(mgr, addr, init_router_, mgr);
    return 1;
}

void init_router_(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        int code;
        char *resp;
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/api/generate-keys")) {
            code = generate_keys(&resp);
        } else if (mg_http_match_uri(hm, "/api/transform-to-same-pk")) {
            code = transform_to_same_pk(&resp);
        } else if (mg_http_match_uri(hm, "/api/transform-back")) {
            code = transform_back(&resp);
        } else {
            resp = mjson_aprintf("{%Q:%Q}", "message", "Invalid URI.");
            code = 403;
        }
        mg_http_reply(c,
                      code,
                      "Content-Type: application/json\r\n",
                      "%s\n",
                      resp);
    }
}
