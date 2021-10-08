/**
 * @file button.h
 * @author Alex Stiles
 * @brief Provides the types and function prototypes required for the button.
 * @version 0.1 alpha
 * @date 2021-08-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef BUTTON_H
#define BUTTON_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCE_DELAY 20 // Debounce delay in ms

typedef struct Button_s Button_t;

struct Button_s {
    /* General */
    uint8_t pin_number;

    /* Debounce */
    long unsigned debounce_delay;
    long unsigned last_trigger_time;

    bool state;
    bool last_state;

    /* Callback */
    void (*callbackFunction)();
};

Button_t init_button(uint8_t pin_number, void (*callbackFunction)());
void button_clicked(Button_t* button);
void debounce_state(Button_t* button, bool read_button_state, long unsigned current_time);

#if defined(__cplusplus)
}
#endif

#endif // BUTTON_H