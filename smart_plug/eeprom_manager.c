#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "eeprom_manager.h"

/**
 * @brief Initialise the program with the information stored in EEPROM
 * 
 * @param eeprom_manager 
 */
void eeprom_manager_init(EEPROM_Manager_t* eeprom_manager)
{
    // The user set flag is stored at EEPROM_USER_SET_ADDR
    bool user_set = eeprom_manager->read_eeprom(EEPROM_USER_SET_ADDR);
    eeprom_manager->user_setup = user_set;

    // The number of presets is stored at EEPROM_PRESET_NUM_ADDR
    uint8_t num_presets = eeprom_manager->read_eeprom(EEPROM_PRESET_NUM_ADDR);
    eeprom_manager->num_presets = num_presets;
}

/* User Storing and Retrieval */

void store_user(EEPROM_Manager_t* eeprom_manager, uint8_t* user_data_buf)
{
    eeprom_manager->write_eeprom(EEPROM_USER_SET_ADDR, true);
    eeprom_manager->write_eeprom(EEPROM_USER_ADDR, user_data_buf[0]);
}

void retrieve_user(EEPROM_Manager_t* eeprom_manager, uint8_t* user_data_buf)
{
    bool temp_rep = eeprom_manager->read_eeprom(EEPROM_USER_ADDR);
    user_data_buf[0] = temp_rep;
}

/* Preset Storing and Retrieval */

void store_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf)
{
    // Write the preset to EEPROM
    for (uint8_t i = 0; i < PRESET_SERIALISED_SIZE; i++) {
        uint8_t preset_id = eeprom_manager->num_presets;
        eeprom_manager->write_eeprom(EEPROM_PRESET_START_ADDR + (PRESET_SERIALISED_SIZE * preset_id) + i, preset_data_buf[i]);
    }

    // Update the number of presets
    eeprom_manager->num_presets++;

    // Write the updated number of presets to EEPROM
    eeprom_manager->write_eeprom(EEPROM_PRESET_NUM_ADDR, eeprom_manager->num_presets);
}

void retrieve_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf, uint8_t preset_id)
{
    // Read in single preset
    for (uint8_t i = 0; i < PRESET_SERIALISED_SIZE; i++) {
        preset_data_buf[i] = eeprom_manager->read_eeprom(EEPROM_PRESET_START_ADDR + (PRESET_SERIALISED_SIZE * preset_id) + i);
    }
}

void retrieve_presets(EEPROM_Manager_t* eeprom_manager, uint8_t* presets_data_buf)
{
    // Read and deserialise all the presets from the EEPROM and add them to the user
    for (size_t i = 0; i < eeprom_manager->num_presets; i++) {
        // Create buffer for single preset
        uint8_t preset_data_buf[PRESET_SERIALISED_SIZE];
        // Get the preset from EEPROM
        retrieve_preset(eeprom_manager, preset_data_buf, i);
        // Add to the all presets buffer
        memcpy(presets_data_buf + (i * PRESET_SERIALISED_SIZE), preset_data_buf, PRESET_SERIALISED_SIZE);
    }
}