#ifndef MANAGER_H
#define MANAGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "connection.h"
#include "user.h"

void handle_action(Connection_t* connection, User_t* user);

#if defined(__cplusplus)
}
#endif

#endif // MANAGER_H