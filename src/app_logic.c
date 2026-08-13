/* Owns routines, sensor-driven rules and per-feature manual overrides. */
#include "app_logic.h"
#include "config.h"
#include "inputs/light_sensor.h"
#include "helper/blinds.h"
#include "active_house_operations/heating_cooling.h"
#include "active_house_operations/morning_routine.h"
#include "active_house_operations/evening_routine.h"
#include "active_house_operations/do_not_disturb.h"

static routine_mode_t routine_from_minutes(unsigned long demo_minutes)
{
    if (demo_minutes < MORNING_START_MINUTES) {
        return ROUTINE_NIGHT;
    }

    if (demo_minutes < DAY_START_MINUTES) {
        return ROUTINE_MORNING;
    }

    if (demo_minutes < EVENING_START_MINUTES) {
        return ROUTINE_DAY;
    }

    if (demo_minutes < NIGHT_START_MINUTES) {
        return ROUTINE_EVENING;
    }

    return ROUTINE_NIGHT;
}

void app_state_init(app_state_t *state)
{
    state->demo_minutes = DEMO_START_MINUTES;
    state->light_raw = 0U;
    state->room_temperature_c = TARGET_TEMPERATURE_C;
    state->target_temperature_c = TARGET_TEMPERATURE_C;
    state->light_level = LIGHT_LEVEL_DARK;
    state->heating_mode = HVAC_COMFORTABLE;
    state->routine = routine_from_minutes(state->demo_minutes);
    state->blind_1 = BLIND_MID;
    state->blind_2 = BLIND_MID;
    state->blinds_manual = 0U;
    state->smart_plug_on = 0U;
    state->smart_plug_manual = 0U;
    state->house_lights_on = 0U;
    state->house_lights_manual = 0U;
    state->dnd_on = 0U;
    state->doorbell_count = 0U;
    state->missed_guests = 0U;

    app_apply_automatic_control(state);
}

void app_update_time(app_state_t *state, unsigned long demo_minutes)
{
    routine_mode_t previous_routine;

    demo_minutes = demo_minutes % MINUTES_PER_DAY;
    previous_routine = state->routine;
    state->demo_minutes = demo_minutes;
    state->routine = routine_from_minutes(demo_minutes);

    if (state->routine != previous_routine) {
        if (state->routine == ROUTINE_MORNING) {
            morning_routine_apply(state);
        }

        if (state->routine == ROUTINE_EVENING) {
            evening_routine_apply(state);
        }
    }

    app_apply_automatic_control(state);
}

void app_update_sensors(app_state_t *state,
                        unsigned int light_raw,
                        unsigned int room_temperature_c)
{
    state->light_raw = light_raw;
    state->light_level = light_sensor_classify_raw(
        light_raw,
        LIGHT_DARK_MAX_RAW,
        LIGHT_BRIGHT_MIN_RAW);
    state->room_temperature_c = room_temperature_c;
    state->heating_mode = heating_cooling_choose_mode(
        room_temperature_c,
        state->target_temperature_c,
        TEMPERATURE_DEADBAND_C);

    app_apply_automatic_control(state);
}

void app_apply_automatic_control(app_state_t *state)
{
    if (state->smart_plug_manual == 0U) {
        if (state->routine == ROUTINE_MORNING) {
            state->smart_plug_on = 1U;
        } else {
            state->smart_plug_on = 0U;
        }
    }

    if (state->blinds_manual == 0U) {
        if ((state->routine == ROUTINE_EVENING) ||
            (state->routine == ROUTINE_NIGHT)) {
            /* Close both blinds at night for privacy and insulation. */
            state->blind_1 = BLIND_DOWN;
            state->blind_2 = BLIND_DOWN;
        } else if ((state->routine == ROUTINE_DAY) &&
                   (state->demo_minutes < AFTERNOON_START_MINUTES)) {
            /*
             * Figure 1 shows Blind 1 on the north-facing wall. Around
             * midday it receives the stronger northern sun, so its position
             * follows the light sensor. The west-facing Blind 2 stays open.
             */
            state->blind_1 = blinds_position_for_light(
                state->light_level);
            state->blind_2 = BLIND_UP;
        } else if (state->routine == ROUTINE_DAY) {
            /*
             * In the afternoon, the west-facing Blind 2 receives the stronger
             * sun. Blind 2 follows the light sensor and Blind 1 stays open.
             */
            state->blind_1 = BLIND_UP;
            state->blind_2 = blinds_position_for_light(
                state->light_level);
        } else {
            /* Morning sun is not directly aimed at either window. */
            state->blind_1 = BLIND_UP;
            state->blind_2 = BLIND_UP;
        }
    }

    if (state->house_lights_manual == 0U) {
        if ((state->routine == ROUTINE_EVENING) ||
            (state->routine == ROUTINE_NIGHT)) {
            state->house_lights_on = 1U;
        } else {
            state->house_lights_on = 0U;
        }
    }
}

void app_toggle_smart_plug(app_state_t *state)
{
    if (state->smart_plug_on == 0U) {
        state->smart_plug_on = 1U;
    } else {
        state->smart_plug_on = 0U;
    }

    state->smart_plug_manual = 1U;
}

void app_cycle_blinds(app_state_t *state)
{
    state->blind_1 = blinds_next_position(state->blind_1);
    state->blind_2 = state->blind_1;
    state->blinds_manual = 1U;
}

void app_toggle_house_lights(app_state_t *state)
{
    if (state->house_lights_on == 0U) {
        state->house_lights_on = 1U;
    } else {
        state->house_lights_on = 0U;
    }

    state->house_lights_manual = 1U;
}

void app_return_to_auto(app_state_t *state)
{
    unsigned long current_demo_minutes;
    unsigned int current_light_raw;
    unsigned int current_room_temperature_c;

    /*
     * AUTO RESET keeps the current environment, but clears user overrides
     * and returns the application outputs to their automatic defaults.
     */
    current_demo_minutes = state->demo_minutes;
    current_light_raw = state->light_raw;
    current_room_temperature_c = state->room_temperature_c;

    app_state_init(state);
    app_update_time(state, current_demo_minutes);
    app_update_sensors(state,
                       current_light_raw,
                       current_room_temperature_c);
}

void app_toggle_dnd(app_state_t *state)
{
    do_not_disturb_toggle(state);
}

void app_record_doorbell(app_state_t *state)
{
    state->doorbell_count++;

    if (state->dnd_on != 0U) {
        state->missed_guests++;
    }
}
