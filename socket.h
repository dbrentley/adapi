//
// Created by dbrent on 8/24/21.
//

#ifndef ADAPI_SOCKET_H
#define ADAPI_SOCKET_H

#include <openssl/ssl.h>
#include "main.h"

int hostname_to_ip(char *hostname, char *ip);

int socket_create(void);

int socket_connect(int h_socket, char *server, int port);

void socket_send(core_t *core, char *request, ...);

void socket_receive(core_t *core);

int open_connection(const char *hostname, int port);

SSL_CTX *init_ctx();

void show_certs(SSL *ssl);

void socket_close(core_t *core);

#endif //ADAPI_SOCKET_H
