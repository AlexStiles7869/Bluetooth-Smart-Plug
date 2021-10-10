#ifndef PRESET_H
#define PRESET_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PRESET_BYTE_SIZE 6

typedef struct {
    uint8_t hour;
    uint8_t minute;
} Time_t;

typedef struct {
    Time_t start_time;
    Time_t end_time;
} TimeBlock_t;

typedef struct {
    // Blank for now
    uint8_t id;
    TimeBlock_t time_block;
    uint8_t trigger_temperature;
} Preset_t;

Preset_t create_preset(uint8_t* packet_buf);

#if defined(__cplusplus)
}
#endif

#endif // PRESET_H