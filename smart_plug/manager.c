#include "manager.h"

#include "response_packet.h"
#include "computer_action_packet.h"
#include "preset.h"

void handle_action(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    Action_Pack_t action = receive_action(connection);

    switch (action.action) {
        case GET_ALL:
            send_presets(connection, user);
            break;
        case GET:
            send_preset(connection, user);
            break;
        case ADD:
            add_preset(connection, user, eeprom_manager);
            break;
        case REMOVE:
            remove_preset(connection, user);
            break;
        default:
            break;
            // Something has gone very, very wrong.
    }    
}

send_presets(Connection_t* connection, User_t* user)
{

}

send_preset(Connection_t* connection, User_t* user)
{

}

add_preset(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    Response_Pack_t response;

    // Create the response packet based on the current number of presets
    if (eeprom_manager->num_presets != MAX_PRESETS) {
        // There is room for another preset, accept the request
        response.response = ACCEPT;
    } else {
        response.response = DENIED;
    }

    // Create the response packet
    uint8_t response_packet_buf[RESPONSE_PACKET_SIZE];
    create_response_packet(&response, response_packet_buf);

    // Send the response packet
    connection->write_bytes_func(response_packet_buf, RESPONSE_PACKET_SIZE);

    if (response.response == ACCEPT) {
        // Get preset data
        uint8_t packet_buf[PRESET_BYTE_SIZE];
        connection->read_bytes_func(packet_buf, PRESET_BYTE_SIZE);
        
        // Create preset from the packet data and add to user
        user->presets[eeprom_manager->num_presets] = create_preset(packet_buf);

        // Store the preset in EEPROM
        store_preset(eeprom_manager, &user->presets[eeprom_manager->num_presets]);
    }
}

remove_preset(Connection_t* connection, User_t* user)
{

}