//
// Created by dbrent on 8/29/21.
//

#include "utils.h"

#include <string.h>

int count_str(char *haystack, char needle) {
    int count = 0;
    for (int x = 0; x < strlen(haystack); x++) {
        if (haystack[x] == needle) {
            count++;
        }
    }
    return count;
}

void split_header(char *header_line, header_t **header, core_t *core) {
    int header_offset = 0;
    for (int x = 0; x < strlen(header_line); x++) {
        if (header_line[x] == ':') {
            header_offset = x + 2;
            char *key = calloc(x, sizeof(char));
            memcpy(key, header_line, x);

            for (int y = 0; y < core->header_count; y++) {
                if (strcmp(header[y]->key, "") == 0) {
                    strncpy(header[y]->key, key, x);
                    strncpy(header[y]->value, header_line + header_offset, strlen(header_line) - header_offset);
                    break;
                }
            }
            free(key);
            break;
        }
    }
}