#ifndef DISPLAY_H
#define DISPLAY_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define LED_COLUMNS 16 + 1 // Number of columns that the LCD display has

#define MAX_ACTIVE_TIME 5000 // Number of the milliseconds that the display should be on for

#define MAX_PAGES 2 // Max number of pages that the LCD can have. Same as the number of enum values in PageName_t

// Names of the pages on the LCD
typedef enum {
    SETUP = 0,
    TIMEBLOCK = 1
} PageName_t;

// The induvidual pages of the LCD
typedef struct {
    PageName_t name;
    char line1[LED_COLUMNS];
    char line2[LED_COLUMNS];
} Page_t;

// Display struct
typedef struct {
    uint8_t num_pages;
    PageName_t current_page;
    Page_t pages[2];

    bool on;
    long unsigned active_time;

} Display_t;

void change_page(PageName_t page_name);

#if defined(__cplusplus)
}
#endif

#endif // DISPLAY_H