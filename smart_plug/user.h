#ifndef USER_H
#define USER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "temperature.h"
#include "preset.h"

#define USER_PACKET_SIZE 1 // The size of the user's packet data (only the preferences)

#define MAX_PRESETS 16 // The max number of presets, this is safely less than the maximum that could be stored before EEPROM memory limit

#define USER_EEPROM_ADDRESS 0 // The address at which the user is stored

typedef struct {
    TemperatureRep_t temp_rep;
} UserPrefs_t;

typedef struct {
    bool user_setup;
    UserPrefs_t user_prefs;
    Preset_t presets[MAX_PRESETS];
} User_t;

void setup_user(User_t* user, uint8_t* user_data_buf);

#if defined(__cplusplus)
}
#endif

#endif // USER_H