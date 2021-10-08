#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "connection.h"
#include "connection_confirm_packet.h"

void connection_attempt_response(Connection_t* connection, User_t* user)
{
    // Read the connection attempt bytes
    uint8_t conn_attempt_buf[CONN_REQUEST_PACK_SIZE];
    connection->read_bytes_func(conn_attempt_buf, CONN_REQUEST_PACK_SIZE);

    // Check if magic number is correct
    uint8_t conn_estab = 0;

    if (conn_attempt_buf[0] == 100) {
        conn_estab = 1;
    }

    // Create the response packet struct
    Conn_Conf_Pack_t conn_conf_pack = {.conn_estab = conn_estab, .error = 0, .setup = !user->user_setup};
    
    // Create buffer
    uint8_t pack_data_buf[CONN_CONF_PACK_SIZE];
    
    // Create packet
    create_conn_conf_packet(&conn_conf_pack, pack_data_buf);

    // Move the data to the HC05 serial buffer to send
    connection->write_bytes_func(pack_data_buf, CONN_CONF_PACK_SIZE);

    // Set connection flag
    connection->currently_connected = true;
}