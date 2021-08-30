//
// Created by dbrent on 8/24/21.
//

#include "logger.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

char *get_level(eLogLevel level) {
    switch (level) {
        case DEBUG: // fine-grained info for debugging
            return "DEBUG";
        case INFO: // informal messages that highlight progress
            return "INFO";
        case WARN: // potentially harmful situations
            return "WARN";
        case ERROR: // event that might allow the application to continue
            return "ERROR";
        case FATAL: // severe error; application abort
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

void logline(eLogLevel level, char *message, ...) {
    time_t now = time(NULL);
    struct tm *ptm = localtime(&now);
    struct timeval tval_now;
    gettimeofday(&tval_now, NULL);

    char *msg = strdup(message);
    message = (char *) malloc(1024 * sizeof(char));
    strcpy(message, msg);

    char buf[30];
    strftime(buf, 30, "%d %b %Y %T", ptm);
    sprintf(message, "%s.%06ld", buf, tval_now.tv_usec);
    sprintf(message + (24 * sizeof(char)), " [%s] %s\n", get_level(level), msg);

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    free(msg);
    free(message);
}
