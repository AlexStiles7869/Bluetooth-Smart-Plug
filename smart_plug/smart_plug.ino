// C++ Libraries
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SoftwareSerial.h>
#include <EEPROM.h>

// #include <Adafruit_GFX.h> Include for Core graphics library
// #include <SPI.h> Include for Software SPI

// C Libraries
#include <stdint.h>

// My Headers
#include "connection.h"
#include "manager.h"
#include "user_manager.h"
#include "preset.h"

// #include "button.h"
#include "display.h"
#include "eeprom_manager.h"

/* Definitions */

// LED
#define ONBOARD_LED_PIN 13

// Button
#define BUTTON_PIN 8

// Display
#define TFT_CS 10
#define TFT_RST 8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 9
#define TFT_BACKLIGHT 7

/* Initialisations */

SoftwareSerial HC05(2, 3); // RX | TX For HC05 bluetooth radio
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// Button_t button;
User_t user;

/* EEPROM Functions */

uint8_t read_eeprom(size_t addr)
{
    return EEPROM.read(addr);
}

void write_eeprom(size_t addr, uint8_t value)
{
    EEPROM.write(addr, value);
}

void delete_eeprom(size_t addr)
{
    EEPROM.write(addr, 0);
}

// Other EEPROM functions

Preset_t read_preset_eeprom(uint8_t preset_id)
{
    Preset_t preset;

    /*
    I'm not passing a Preset_t pointer to EEPROM.get() because there is apparently some strange C++ syntax for passing 'by reference' which
    is different to passing a 'reference' (a pointer address). This caused me a large headache so I'm going to accept it and move on.
    Read more here: https://community.particle.io/t/pass-pointer-to-eeprom-get-put/57123/6
    */

    EEPROM.get(EEPROM_PRESET_START_ADDR + (PRESET_BYTE_SIZE * preset_id), preset);

    return preset
}

void write_preset_eeprom(Preset_t preset)
{
    EEPROM.put(EEPROM_PRESET_START_ADDR + (PRESET_BYTE_SIZE * preset.id), preset);
}


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

/*

// You need to have the MOSFET switching circuit to enable this, see report.

void toggle_display(Display_t* display)
{
    Serial.println(!display->on);
    // Toggle the display on and off when the button is pressed
    digitalWrite(display->backlight_pin, !display->on);

    // Toggle the on state of the OLED
    display->on = !display->on;

    // Reset the active time
    display->active_time = 0;
}
*/

void update_display(Display_t* display) {
    // Update the displayed contents of the OLED display
    
    if (display->page_changed || display->content_changed) {
        // Clear the screen
        tft.fillScreen(ST77XX_BLACK);

        // Reset the curosr
        tft.setCursor(0, 0);

        // Print the first line
        tft.println(display->pages[display->current_page].line1);

        // Print the second line
        tft.println(display->pages[display->current_page].line2);

        display->page_changed = false;
        display->content_changed = false;
    }
}

/* More Initialisations that require functions */

Connection_t connection = {.currently_connected = false, .read_bytes_func = hc05_read_bytes, .write_bytes_func = hc05_write_bytes};
Display_t display = {
    .backlight_pin = TFT_BACKLIGHT,
    .num_pages = 2, 
    .current_page = 0, 
    /*
    The wonderful piece of code below which defies all beleive, and more importantly, the C99 standard
    seems to be the result of a parser error. See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55227
    */
    .pages = {{.name = SETUP, {.line1 = "Please Setup"}, {.line2 = ""}}, /* Wierd that I need braces around .line1 and .line2 */
    {.name = NO_PRESETS, {.line1 = "No Presets"}, {.line2 = ""}},
    {.name = TIMEBLOCK, {.line1 = ""}, {.line2 = ""}}},
    .on = false, 
    .page_changed = false,
    .content_changed = false,
    .active_time = 0,
    .toggle_display = NULL,
    .update_display = update_display
};
EEPROM_Manager_t eeprom_manager = {
    .user_setup = false,
    .num_presets = 0,
    .read_eeprom = read_eeprom,
    .write_eeprom = write_eeprom,
    .delete_eeprom = delete_eeprom
};

void setup()
{
    // for (int i = 0; i < 512; i++) {
    //     EEPROM.write(i, 0);
    // }
    // Start USB serial
    Serial.begin(9600);

    // Start HC-05 serial
    HC05.begin(9600);
    // Set timeout to an hour
    HC05.setTimeout(60 * 60 * 1000);

    // Inititalise display
    tft.initR(INITR_BLACKTAB);

    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(1);
    tft.setCursor(0, 0);
    tft.setTextSize(2);

    // Initialise the EEPROM manager
    eeprom_manager_init(&eeprom_manager);

    // Read in data from EEPROM relating to user prefs (or if setup needed) and current presets
    user.user_setup = eeprom_manager.user_setup;

    if (user.user_setup) {
        // Read all the presets and provide to user
        get_presets(&eeprom_manager, user.presets);

        if (eeprom_manager.num_presets == 0) {
          change_page(&display, NO_PRESETS);
          update_display(&display);
        }
    } else {
        // Display that setup needs to be completed
        change_page(&display, SETUP);
        update_display(&display);  
    }

    // Inititalise button
    // button = init_button(8, toggle_display);
}


void loop()
{
    // Time in milliseconds that the Arduino has been on for
    // long unsigned time_on = millis();

    // Poll the HC-05 serial buffer to see if computer wants to connect
    if (HC05.available() > 0) {
        // Check if connection has been established with computer
        if (!connection.currently_connected) {
            // Send appropriate response depending on valid magic number, general errors, and setup state
            connection_attempt_response(&connection, &user);

            if (!user.user_setup) {
                // Create user based on computer data
                create_user(&connection, &user);
                
                // Store the newly created user in the EEPROM
                store_user(&eeprom_manager, &user);

                // Change the page to no presets
                change_page(&display, NO_PRESETS);
            }
        } else {
            // Everything is connected and setup, handle commands from computer
            handle_action(&connection, &user, &eeprom_manager);
        }
    }

    // Update display contents if required
    update_display(&display);  

    // Poll the temperature sensor
    
    /* 
    If you had an appropriate switchable power source for the ST7735's backlight, you can 
    uncomment the below code and add a button to toggle the display, with a 5 second inactivity timeout.

    // Poll the button to turn toggle display
    bool button_state = digitalRead(BUTTON_PIN);

    debounce_state(&button, button_state, time_on);

    // Check to see if display has timed out (if it is on)
    poll_display_timeout(&display, time_on);
    */  
}
