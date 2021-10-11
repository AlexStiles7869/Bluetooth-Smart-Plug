#include "user_manager.h"
#include "eeprom_manager.h"
#include "preset_manager.h"
#include "core_controller.h"

#include "response_packet.h"
#include "action_packet.h"

/* User Setup */

void setup_saved_user(User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    // Read the user's temperature preference from EEPROM
    uint8_t user_data_buf[USER_SERIALISED_SIZE];
    retrieve_user(eeprom_manager, user_data_buf);

    // Deserialise the user
    deserialise_user(user, user_data_buf);

    // Create buffer for all presets and read them in from EEPROM
    // I could replace MAX_PRESETS with eeprom_manager->num_presets to get an exactly sized array but that would require using a VLA
    uint8_t presets_data_buf[MAX_PRESETS * PRESET_SERIALISED_SIZE];
    retrieve_presets(eeprom_manager, presets_data_buf);

    // Add presets to the user
    for (uint8_t i = 0; i < eeprom_manager->num_presets; i++) {
        user->presets[i] = deserialise_preset(eeprom_manager, presets_data_buf + (i * PRESET_SERIALISED_SIZE));
    }
}

void setup_new_user(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    // Receive user data and create locally
    create_user(connection, user);

    // Serialise the user
    uint8_t user_data_buf[USER_SERIALISED_SIZE];
    serialise_user(user, user_data_buf);

    // Store the newly created user in the EEPROM
    store_user(eeprom_manager, user_data_buf);
}

void send_user_data(Connection_t* connection, User_t* user)
{
    // Serialise the user
    uint8_t user_data_buf[USER_SERIALISED_SIZE];
    serialise_user(user, user_data_buf);

    // Send the user's data to the computer
    connection->write_bytes_func(user_data_buf, USER_SERIALISED_SIZE);
}

/* General Actions */

void receive_action(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    Action_Pack_t action = receive_action_packet(connection);

    switch (action.action) {
        case GET_ALL:
            // send_presets(connection, user);
            break;
        case GET:
            // send_preset(connection, user);
            break;
        case ADD:
            add_preset(connection, user, eeprom_manager);
            break;
        case REMOVE:
            // remove_preset(connection, user);
            break;
        default:
            break;
            // Something has gone very, very wrong.
    }    
}

