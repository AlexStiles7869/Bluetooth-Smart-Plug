#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "connection.h"

// Packets
#include "connection_confirm_packet.h"

/**
 * @brief Determines if the incoming connection is valid, in this case by checking if the magic number is correct. Returns true if the connection is valid, else false.
 * 
 * @return true 
 * @return false 
 */
bool valid_connection(uint8_t* conn_attempt_packet)
{
    return conn_attempt_packet[0] == CONN_MAGIC_NUM;
}

/**
 * @brief Receives and responds to a connection request.
 * 
 * @param connection The connection with the HC-05 bluetooth radio. Used to write and read bytes from the radio.
 * @param user The user of the smart plug. Need to know if they are setup or not.
 */
void establish_connection(Connection_t* connection, User_t* user)
{
    // Read the connection request packet bytes
    uint8_t connection_request_buf[CONN_REQUEST_PACK_SIZE];
    connection->read_bytes_func(connection_request_buf, CONN_REQUEST_PACK_SIZE);

    // Check if magic number is correct
    uint8_t conn_estab = valid_connection(connection_request_buf);

    // Create the response packet struct
    Conn_Conf_Pack_t connection_response_pack = {.conn_estab = conn_estab, .error = 0, .setup = !user->user_setup};
    
    // Create buffer
    uint8_t connection_response_buf[CONN_CONF_PACK_SIZE];
    
    // Create packet and send it
    create_connection_packet(&connection_response_pack, connection_response_buf);
    connection->write_bytes_func(connection_response_buf, CONN_CONF_PACK_SIZE);

    if (conn_estab) {
        // Set connection flag
        connection->currently_connected = true;
    }
}