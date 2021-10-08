#include <stdint.h>

#include "response_packet.h"

void create_response_packet(Response_Pack_t* packet, uint8_t* packet_buf)
{
       packet_buf[0] = packet->response;
}
