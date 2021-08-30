//
// Created by dbrent on 8/24/21.
//

#ifndef ADAPI_LOGGER_H
#define ADAPI_LOGGER_H

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
} eLogLevel;

void logline(eLogLevel level, char *message, ...);

#endif //ADAPI_LOGGER_H
