#ifndef CONNECTION_H
#define CONNECTION_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "user.h"

typedef struct {
    bool currently_connected;
    uint8_t (*read_bytes_func)(uint8_t* buf, size_t size);
    size_t (*write_bytes_func)(uint8_t* buf, size_t size);
} Connection_t;

void connection_attempt_response(Connection_t* connection, User_t* user);

#if defined(__cplusplus)
}
#endif

#endif // CONNECTION_H