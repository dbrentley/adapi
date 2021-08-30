//
// Created by dbrent on 8/24/21.
//

#include "socket.h"
#include "logger.h"
#include "main.h"
#include "utils.h"

#if defined(__linux__)

#include <arpa/inet.h>

#elif defined(__APPLE__)
#include <pcap/socket.h>
#endif

#include <zconf.h>
#include <stdio.h>
#include <stdarg.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


void socket_send(core_t *core, char *request, ...) {
    char *msg = (char *) calloc(4096, sizeof(char));
    if (msg == NULL) {
        logline(FATAL, "Could not allocate memory for message.");
        exit(-1);
    }

    va_list args;
    va_start(args, request);
    vsprintf(msg, request, args);
    va_end(args);

    SSL_write(core->ssl, msg, (int) strlen(msg));

    core->request_string = msg;
    free(msg);
}

void socket_receive(core_t *core) {
    char data[8196] = {0};
    char header[8196] = {0};

    SSL_read(core->ssl, data, sizeof(data));

    char *tok = strstr(data, "\r\n\r\n");
    if (tok) {
        *tok = '\0';
    }

    char *p;
    int header_size = 0;
    for (p = data; *p; p++) {
        header_size++;
    }
    core->header_size = header_size;
    strncpy(header, data, header_size);

    int header_lines = count_str(header, '\n');
    core->headers = malloc(header_lines * sizeof(header_t));
    core->header_count = header_lines - 1; // -1 for status line
    for (int n = 0; n < header_lines; n++) {
        core->headers[n] = malloc(sizeof(header_t));
        core->headers[n]->key = calloc(2048, sizeof(char));
        core->headers[n]->value = calloc(2048, sizeof(char));
    }

    char *h = calloc(header_size, sizeof(char));
    int position = 0;
    for (int x = 0; x < strlen(header); x++) {
        if (header[x] == '\r') {
            memcpy(h, header + position, x - position);
            split_header(h, core->headers, core);
            free(h);
            h = calloc(header_size, sizeof(char));
            position = x += 2;
        }
    }
    if (h != NULL) {
        free(h);
    }
}

int open_connection(const char *hostname, int port) {
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    if ((host = gethostbyname(hostname)) == NULL) {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long *) (host->h_addr);
    if (connect(sd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

SSL_CTX *init_ctx() {
    SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLSv1_2_client_method();
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void socket_close(core_t *core) {
    SSL_free(core->ssl);
    SSL_CTX_free(core->ctx);
    close(core->handle);
}
