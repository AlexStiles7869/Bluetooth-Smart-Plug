#include <stdio.h>
#include <string.h>

#include "display.h"

void display_preset(Display_t* display, User_t* user, Preset_t* preset, bool current)
{
    char preset_details[LED_COLUMNS];

    sprintf(preset_details, "From: %02d:%02d\nTo: %02d:%02d\n%d%s", 
        preset->time_block.start_time.hour,
        preset->time_block.start_time.minute,
        preset->time_block.end_time.hour,
        preset->time_block.end_time.minute,
        preset->trigger_temperature,
        user->user_prefs.temp_rep ? "F" : "C"
    );
    char* lines[] = {current ? "Current Preset:" : "Up Next:", preset_details};

    change_page(display, TIMEBLOCK);
    change_content(display, lines);
}

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
            display->toggle_display(display);
        } else {
            // Update the active time of the LCD
            display->active_time = time_on - display->active_time;
        }
    }
}