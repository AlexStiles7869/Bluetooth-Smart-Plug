#include <stdbool.h>

#include "user.h"

void setup_user(User_t* user, uint8_t* user_data_buf)
{
    // Set the user's temperature preferences
    user->user_prefs.temp_rep = user_data_buf[0];

    // User setup complete
    user->user_setup = true;
}