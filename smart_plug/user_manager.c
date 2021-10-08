#include <stdint.h>

#include "user_manager.h"

#include "computer_action_packet.h"
#include "response_packet.h"

void get_user_data(Connection_t* connection, uint8_t* packet_buf)
{
    Action_Pack_t comptuer_action = receive_action(connection);
    Response_Pack_t response;

    // Determine if the request is valid 
    if (comptuer_action.action == ADD_USER) {
        response.response = ACCEPT;
    } else {
        response.response = DENIED;
    }

    // Create the response packet
    uint8_t response_packet_buf[RESPONSE_PACKET_SIZE];
    create_response_packet(&response, response_packet_buf);

    // Send the response packet
    connection->write_bytes_func(response_packet_buf, RESPONSE_PACKET_SIZE);

    // Get user data
    connection->read_bytes_func(packet_buf, USER_PACKET_SIZE);
}

void create_user(Connection_t* connection, User_t* user)
{
    // Get the user data from the computer
    uint8_t user_data[USER_PACKET_SIZE];
    get_user_data(connection, user_data);

    // Setup the user on the Arduino based on the sent computer data
    setup_user(user, user_data);
}