#ifndef MANAGER_H
#define MANAGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "connection.h"

// Managers
#include "user_manager.h"
#include "eeprom_manager.h"
#include "preset_manager.h"

// Types
#include "user.h"

void setup_saved_user(User_t* user, EEPROM_Manager_t* eeprom_manager);
void setup_new_user(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager);
void send_user_data(Connection_t* connection, User_t* user);

void receive_action(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager);

#if defined(__cplusplus)
}
#endif

#endif // MANAGER_H