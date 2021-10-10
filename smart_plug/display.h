#ifndef DISPLAY_H
#define DISPLAY_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define LED_COLUMNS 16 + 1 // Number of columns that the LCD display has

#define MAX_ACTIVE_TIME 5000 // Number of the milliseconds that the display should be on for

#define MAX_PAGES 3 // Max number of pages that the LCD can have. Same as the number of enum values in PageName_t

// Names of the pages on the LCD
typedef enum {
    SETUP = 0,
    NO_PRESETS = 1,
    TIMEBLOCK = 2
} PageName_t;

// The induvidual pages of the LCD
typedef struct {
    PageName_t name;
    char line1[LED_COLUMNS];
    char line2[LED_COLUMNS];
} Page_t;

typedef struct Display_s Display_t;

// Display struct
struct Display_s {
    uint8_t backlight_pin;
    
    uint8_t num_pages;
    PageName_t current_page;
    Page_t pages[MAX_PAGES];

    bool on;
    bool page_changed;
    bool content_changed;
    long unsigned active_time;

    // Management Functions
    void (*toggle_display)(Display_t* display);
    void (*update_display)(Display_t* display);

};

void change_page(Display_t* display, PageName_t page_name);
void change_content(Display_t* display, char** lines);
void poll_display_timeout(Display_t* display, long unsigned time_on);

#if defined(__cplusplus)
}
#endif

#endif // DISPLAY_H