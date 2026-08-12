#ifndef DESN2000_APP_STATE_H
#define DESN2000_APP_STATE_H

typedef enum {
    BLIND_UP = 0,
    BLIND_MID,
    BLIND_DOWN
} blind_position_t;

typedef enum {
    LIGHT_LEVEL_DARK = 0,
    LIGHT_LEVEL_MEDIUM,
    LIGHT_LEVEL_BRIGHT
} light_level_t;

typedef enum {
    HVAC_HEATING = 0,
    HVAC_COMFORTABLE,
    HVAC_COOLING
} heating_mode_t;

typedef enum {
    ROUTINE_MORNING = 0,
    ROUTINE_DAY,
    ROUTINE_EVENING,
    ROUTINE_NIGHT
} routine_mode_t;

typedef enum {
    TOUCH_NONE = 0,
    TOUCH_SMART_PLUG,
    TOUCH_BLINDS,
    TOUCH_RETURN_AUTO,
    TOUCH_DND,
    TOUCH_HOUSE_LIGHTS
} touch_command_t;

typedef struct {
    unsigned long demo_minutes;
    unsigned int light_raw;
    unsigned int room_temperature_c;
    unsigned int target_temperature_c;
    light_level_t light_level;
    heating_mode_t heating_mode;
    routine_mode_t routine;
    blind_position_t blind_1;
    blind_position_t blind_2;
    unsigned int blinds_manual;
    unsigned int smart_plug_on;
    unsigned int smart_plug_manual;
    unsigned int house_lights_on;
    unsigned int house_lights_manual;
    unsigned int dnd_on;
    unsigned int doorbell_count;
    unsigned int missed_guests;
} app_state_t;

#endif
