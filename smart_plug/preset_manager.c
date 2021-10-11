#include "preset_manager.h"

// Packets
#include "response_packet.h"

void add_preset(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)
{
    // Send a response to the computer based on if the request was valid
    Response_Pack_t response_pack = {.response = eeprom_manager->num_presets < MAX_PRESETS};

    uint8_t response_packet_buf[RESPONSE_PACKET_SIZE];
    create_response_packet(&response_pack, response_packet_buf);
    connection->write_bytes_func(response_packet_buf, RESPONSE_PACKET_SIZE);

    // Receive the preset from the computer
    uint8_t preset_data_buf[PRESET_SERIALISED_SIZE];
    connection->read_bytes_func(preset_data_buf, PRESET_SERIALISED_SIZE);

    // Deserialise preset and add to user
    Preset_t preset = deserialise_preset(eeprom_manager, preset_data_buf);
    user->presets[eeprom_manager->num_presets] = preset;

    // Store the preset in EEPROM
    store_preset(eeprom_manager, preset_data_buf);
}

Preset_t deserialise_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf)
{
    Preset_t preset = {
        .id = eeprom_manager->num_presets,
        .time_block = {
            {.hour = preset_data_buf[0], .minute = preset_data_buf[1]},
            {.hour = preset_data_buf[2], .minute = preset_data_buf[3]}
        },
        .trigger_temperature = preset_data_buf[4]
    };

    return preset;
}