#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "user.h"

#define EEPROM_USER_SET_ADDR 0
#define EEPROM_PRESET_NUM_ADDR 1
#define EEPROM_USER_ADDR 2
#define EEPROM_PRESET_START_ADDR 3

typedef struct EEPROM_Manager_s {
    bool user_setup;
    uint8_t num_presets;

    uint8_t (*read_eeprom)(size_t addr);
    void (*write_eeprom)(size_t addr, uint8_t value);
    void (*delete_eeprom)(size_t addr);
} EEPROM_Manager_t;

void eeprom_manager_init(EEPROM_Manager_t* eeprom_manager); // Reads the current EEPROM and determines if the user has been set and what presets there are
void store_user(EEPROM_Manager_t* eeprom_manager, uint8_t* user_data_buf); // Stores the user (excluding presets)
void retrieve_user(EEPROM_Manager_t* eeprom_manager, uint8_t* user_data_buf);
void store_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf);
void retrieve_preset(EEPROM_Manager_t* eeprom_manager, uint8_t* preset_data_buf, uint8_t preset_id);
void retrieve_presets(EEPROM_Manager_t* eeprom_manager, uint8_t* presets_data_buf);

/* Editing functionality has been not been implemented, not enough time */

#if defined(__cplusplus)
}
#endif


#endif // EEPROM_MANAGER_H