#ifndef RESPONSE_PACKET_H
#define RESPONSE_PACKET_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define RESPONSE_PACKET_SIZE 1

typedef enum {
    CONN_TERMINATED = 0,
    ACCEPT = 1,
    DENIED = 2
} Response_t;

typedef struct {
    Response_t response;
} Response_Pack_t;

void create_response_packet(Response_Pack_t* packet, uint8_t* packet_buf);

#if defined(__cplusplus)
}
#endif

#endif // RESPONSE_PACKET_H