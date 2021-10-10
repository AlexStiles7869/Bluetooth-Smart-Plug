#include <stdbool.h>
#include <stdint.h>

#include "eeprom_manager.h"

void eeprom_manager_init(EEPROM_Manager_t* eeprom_manager)
{
    // The user set flag is stored at EEPROM_USER_SET_ADDR
    bool user_set = eeprom_manager->read_eeprom(EEPROM_USER_SET_ADDR);
    eeprom_manager->user_setup = user_set;

    // The number of presets is stored at EEPROM_PRESET_NUM_ADDR
    uint8_t num_presets = eeprom_manager->read_eeprom(EEPROM_PRESET_NUM_ADDR);
    eeprom_manager->num_presets = num_presets;
}

void store_user(EEPROM_Manager_t* eeprom_manager, User_t* user)
{
    /*
    This process should be more abstracted by having a serialise function for the user,
    but this will have to do for now. There's a lot left to do.
    */
    eeprom_manager->write_eeprom(EEPROM_USER_SET_ADDR, true);
    eeprom_manager->write_eeprom(EEPROM_USER_ADDR, user->user_prefs.temp_rep);
}

void store_preset(EEPROM_Manager_t* eeprom_manager, Preset_t* preset)
{
    // Write the preset to the EEPROM
    write_preset_eeprom(*preset);

    // Increment the number of presets
    eeprom_manager->num_presets++;

    // Write the updated preset amount to the EEPROM
    eeprom_manager->write_eeprom(EEPROM_PRESET_NUM_ADDR, eeprom_manager->num_presets);
}

User_t get_user(EEPROM_Manager_t* eeprom_manager)
{
    /*
    Again, user.c should have a function to create a user from the serialised data
    but I'm doing it here to move along faster.
    */
    bool temp_rep = eeprom_manager->read_eeprom(EEPROM_USER_ADDR);
    User_t user = {.user_prefs = {.temp_rep = temp_rep}};

    return user;
}

Preset_t get_preset(uint8_t preset_id)
{
    // Read the preset from EEPROM
    Preset_t preset = read_preset_eeprom(preset_id);

    return preset;
}

uint8_t get_presets(EEPROM_Manager_t* eeprom_manager, Preset_t* preset_container)
{
    for (uint8_t i = 0; i < eeprom_manager->num_presets; i++) {
        preset_container[i] = get_preset(i);
    }
}