#include <stdint.h>
#include <stdlib.h>

#include "connection_confirm_packet.h"

void create_connection_packet(Conn_Conf_Pack_t* packet, uint8_t* packet_buf)
{
    packet_buf[0] = packet->conn_estab;
    packet_buf[1] = packet->error;
    packet_buf[2] = packet->setup;
}