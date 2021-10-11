#include <stdint.h>

#include "connection.h"
#include "action_packet.h"

Action_Pack_t receive_action_packet(Connection_t* connection)
{
    // Create buffer for user action
    uint8_t action_bytes_buf[COMPUTER_ACTION_PACKET_SIZE];
    
    // Get the user action from the computer
    connection->read_bytes_func(action_bytes_buf, COMPUTER_ACTION_PACKET_SIZE);

    // Create user action struct
    Action_Pack_t action = {.action = action_bytes_buf[0]};

    return action;
}

void send_action_packet(Action_Pack_t* action_packet)
{
    
}