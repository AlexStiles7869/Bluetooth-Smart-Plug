#include "manager.h"

#include "computer_action_packet.h"

void handle_action(Connection_t* connection, User_t* user)
{
    Action_Pack_t action = receive_action(connection);

    
}