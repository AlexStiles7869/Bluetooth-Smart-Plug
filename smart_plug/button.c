#include <stdint.h>
#include <stdbool.h>

#include "button.h"

Button_t init_button(uint8_t pin_number, void (*callbackFunction)()) {
    Button_t button = {
        .pin_number = pin_number, 
        .debounce_delay = DEBOUNCE_DELAY, 
        .last_trigger_time = 0, 
        .state = true, 
        .last_state = true, 
        .callbackFunction = callbackFunction
    };

    return button;
}

void button_clicked(Button_t* button) {
    button->callbackFunction();
}

void debounce_state(Button_t* button, bool read_button_state, long unsigned current_time) {    
    if (read_button_state != button->last_state) {
        button->last_trigger_time = current_time;
    }

    if ((current_time - button->last_trigger_time) > button->debounce_delay) {
        if (read_button_state != button->state) {
            button->state = read_button_state;
            if (button->state) {
                button_clicked(button);
            }
        }
    }

    button->last_state = read_button_state;
}