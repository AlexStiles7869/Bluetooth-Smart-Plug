#ifndef COMPUTER_ACTION_PACKET_H
#define COMPUTER_ACTION_PACKET_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "connection.h"

#define COMPUTER_ACTION_PACKET_SIZE 1

typedef enum {
    TERMINATE_CONN = 0,
    ADD_USER = 1,
    EDIT_USER = 2,
    GET_ALL = 3,
    GET = 4,
    ADD = 5,
    EDIT = 6,
    REMOVE = 7
} Action_t;

typedef struct {
    Action_t action;
} Action_Pack_t;

Action_Pack_t receive_action(Connection_t* connection);

#if defined(__cplusplus)
}
#endif

#endif // COMPUTER_ACTION_PACKET_H