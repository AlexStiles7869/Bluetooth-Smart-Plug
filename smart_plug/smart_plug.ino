// C++ Libraries
#include <SoftwareSerial.h>

// C Libraries
#include <stdint.h>

// My Headers
#include "connection.h"
#include "manager.h"
#include "user_manager.h"

#include "button.h"
#include "display.h"

// Definitions
#define ONBOARD_LED_PIN 13
#define BUTTON_PIN 8

/* Initialisations */

SoftwareSerial HC05(10, 11); // RX | TX
Button_t button;
User_t user;

Display_t display = {
    .num_pages = 2, 
    .current_page = 0, 
    /*
    The wonderful piece of code below which defies all beleive, and more importantly, the C99 standard
    seems to be the result of a parser error. See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55227
    */
    {{.name = 0, {.line1 = "Please setup"}, {.line2 = ""}} /* Wierd that I need braces around .line1 and .line2 */ ,
    {.name = 1} }, 
    .on = false, 
    .active_time = 0
};

/* EEPROM Functions */

void write_eeprom();

void update_eeprom();

void delete_eeprom();

/* Serial Reading / Writing */

int read_count = 0;
int write_count = 0;

uint8_t hc05_read_bytes(uint8_t* buf, size_t size)
{
    uint8_t bytes_read = HC05.readBytes(buf, size);

    Serial.print("Read Count: ");
    Serial.print(read_count++);
    Serial.print(" | ");
    for (size_t i = 0; i < size; i++) {
        Serial.print(buf[i], DEC);
        Serial.print(" ");
    }
    Serial.println();

    return bytes_read;
}

size_t hc05_write_bytes(uint8_t* buf, size_t size)
{
    size_t bytes_written = HC05.write(buf, size);

    Serial.print("Write Count: ");
    Serial.print(write_count++);
    Serial.print(" | ");
    for (size_t i = 0; i < size; i++) {
        Serial.print(buf[i], DEC);
        Serial.print(" ");
    }
    Serial.println();
    return bytes_written;
}

/* Button Callback and Display Controls */

void toggle_display()
{
    // Toggle the display on and off when the button is pressed
    display.active_time = 0;
}

void update_display(char* string, uint8_t row)
{
    // Update the displayed contents of the LCD display
}

/* More Initialisations that require functions */

Connection_t connection = {.currently_connected = false, .read_bytes_func = hc05_read_bytes, .write_bytes_func = hc05_write_bytes};

void setup()
{
    // Start USB serial
    Serial.begin(9600);
    // Start HC-05 serial
    HC05.begin(9600);
    // Set timeout to an hour
    HC05.setTimeout(60 * 60 * 1000);

    // Inititalise button
    button = init_button(8, toggle_display);

    // Read in data from EEPROM relating to user prefs (or if setup needed) and current presets
    user.user_setup = false;
}


void loop()
{
    // Time in milliseconds that the Arduino has been on for
    long unsigned time_on = millis();

    // Poll the HC-05 serial buffer to see if computer wants to connect
    if (HC05.available() > 0) {
        // Check if connection has been established with computer
        if (!connection.currently_connected) {
            // Send appropriate response depending on valid magic number, general errors, and setup state
            connection_attempt_response(&connection, &user);

            if (!user.user_setup) {
                // Create user based on computer data
                create_user(&connection, &user);
            }
        } else {
            // Everything is connected and setup, handle commands from computer
            handle_action(&connection, &user);
        }
    }

    // Poll the temperature sensor
    

    // Poll the button to turn toggle display
    bool button_state = digitalRead(BUTTON_PIN);

    debounce_state(&button, button_state, time_on);

    // If display is on, check that it hasn't been for more than 5 seconds
    if (display.on && display.active_time > MAX_ACTIVE_TIME) {
        // Toggle the LCD (turning it off)
        toggle_display();
    } else {
        // Update the active time of the LCD
        display.active_time = time_on - display.active_time;
    }
}
