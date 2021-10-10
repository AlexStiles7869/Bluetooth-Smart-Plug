#include <string.h>

#include "display.h"

void change_content(Display_t* display, char** lines)
{
    strcpy(display->pages[display->current_page].line1, lines[0]);
    strcpy(display->pages[display->current_page].line2, lines[1]);

    display->content_changed = true;
}

void change_page(Display_t* display, PageName_t page_name)
{
    display->current_page = page_name;

    display->page_changed = true;
}

void poll_display_timeout(Display_t* display, long unsigned time_on)
{
    // If display is on, check that it hasn't been for more than 5 seconds
    if (display->on) {
        if (display->active_time > MAX_ACTIVE_TIME) {
            // Toggle the LCD (turning it off)
            display->toggle_display(&display);
        } else {
            // Update the active time of the LCD
            display->active_time = time_on - display->active_time;
        }
    }
}