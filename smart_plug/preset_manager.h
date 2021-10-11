#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#include "connection.h"
#include "user_manager.h"
#include "eeprom_manager.h"

#include "preset.h"

void add_preset(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager);
Preset_t deserialise_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf);

#endif // PRESET_MANAGER_H