#include <stdint.h>

#include "preset.h"

Preset_t create_preset(uint8_t* packet_buf)
{
    Preset_t preset = {
        .id = packet_buf[0],
        .time_block = {
            {.hour = packet_buf[1], .minute = packet_buf[2]},
            {.hour = packet_buf[3], .minute = packet_buf[4]}
        },
        .trigger_temperature = packet_buf[5]
    };

    return preset;
}