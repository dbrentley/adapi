#include "logger.h"
#include "socket.h"
#include "main.h"

#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>


int main() {
    core_t *core = malloc(sizeof(core_t));
    if (core == NULL) {
        printf("Could not allocate memory for core\n");
        exit(-1);
    }
    core->ssl = NULL;
    core->ctx = NULL;
    core->headers = NULL;
    core->request_string = NULL;
    core->request = NULL;
    core->response = NULL;
    core->rec_bytes = 0;
    core->handle = 0;
    core->header_size = 0;
    core->header_count = 0;

    char *hostname = "www.google.com";

    SSL_library_init();
    core->ctx = init_ctx();
    core->ssl = SSL_new(core->ctx);

    core->handle = open_connection(hostname, 443);
    SSL_set_fd(core->ssl, core->handle);
    if (SSL_connect(core->ssl) == -1) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    socket_send(core, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", hostname);
    socket_receive(core);

    // print headers
    for (int x = 0; x < core->header_count; x++) {
        printf("Key: %s, Value: %s\n", core->headers[x]->key, core->headers[x]->value);
    }

    socket_close(core);
    return 0;
}
