#ifndef MANAGER_H
#define MANAGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "connection.h"
#include "user.h"
#include "eeprom_manager.h"

void handle_action(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager)

void add_preset(Connection_t* connection, User_t* user, EEPROM_Manager_t* eeprom_manager);

#if defined(__cplusplus)
}
#endif

#endif // MANAGER_H