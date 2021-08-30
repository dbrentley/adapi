//
// Created by dbrent on 8/26/21.
//

#ifndef ADAPI_MAIN_H
#define ADAPI_MAIN_H

#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct {
    char *key;
    char *value;
} header_t;

typedef struct {
    SSL_CTX *ctx;
    SSL *ssl;
    int handle;
    int rec_bytes;
    int header_size;
    int header_count;
    header_t **headers;
    char *request_string;
    char *request_path;
    char *request;
    char *response;
} core_t;

#endif //ADAPI_MAIN_H
