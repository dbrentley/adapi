//
// Created by dbrent on 8/29/21.
//

#include "main.h"

#ifndef ADAPI_UTILS_H
#define ADAPI_UTILS_H

int count_str(char *haystack, char needle);

void split_header(char *header_line, header_t **header, core_t *core);

#endif //ADAPI_UTILS_H
