#ifndef CONNECTION_CONFIRM_PACKET_H
#define CONNECTION_CONFIRM_PACKET_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define CONN_REQUEST_PACK_SIZE 1
#define CONN_CONF_PACK_SIZE 3 // Number of bytes in connection confirmation packet

typedef struct {
    uint8_t conn_estab;
    uint8_t error;
    uint8_t setup;
} Conn_Conf_Pack_t;

void create_connection_packet(Conn_Conf_Pack_t* packet, uint8_t* packet_buf);

#if defined(__cplusplus)
}
#endif

#endif // CONNECTION_CONFIRM_PACKET_H