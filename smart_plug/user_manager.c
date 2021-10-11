#include <stdint.h>
#include <stdbool.h>

#include "user_manager.h"

// Packets
#include "action_packet.h"
#include "response_packet.h"

/**
 * @brief Create a user from data received from the computer
 * 
 * @param connection Current connection with HC-05 bluetooth radio
 * @param user User of the smart plug
 */
void create_user(Connection_t* connection, User_t* user)
{
    // Receive the request packet
    Action_Pack_t comptuer_action = receive_action_packet(connection);
    
    // Determine if the request is valid 
    Response_Pack_t response = {.response = comptuer_action.action == ADD_USER ? ACCEPT : DENIED};
    
    // Create the response packet
    uint8_t response_packet_buf[RESPONSE_PACKET_SIZE];
    create_response_packet(&response, response_packet_buf);

    // Send the response packet
    connection->write_bytes_func(response_packet_buf, RESPONSE_PACKET_SIZE);

    // Get user data
    uint8_t user_data_buf[USER_SERIALISED_SIZE];
    connection->read_bytes_func(user_data_buf, USER_SERIALISED_SIZE);

    // Setup the user on the Arduino based on the sent computer data
    deserialise_user(user, user_data_buf);
}

void serialise_user(User_t* user, uint8_t* user_data_buf)
{
    user_data_buf[0] = user->user_prefs.temp_rep;
}

void deserialise_user(User_t* user, uint8_t* user_data_buf)
{
    user->user_prefs.temp_rep = user_data_buf[0];
    user->user_setup = true;
}