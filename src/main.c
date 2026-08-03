#include "main.h"
#include "config.h"
#include "app_state.h"
#include "app_logic.h"
#include "inputs/light_sensor.h"
#include "inputs/dial_temp.h"
#include "inputs/doorbell_button.h"
#include "inputs/smart_plug_button.h"
#include "inputs/touch_input.h"
#include "helper/timebase.h"
#include "helper/audio.h"
#include "helper/blinds.h"
#include "active_house_operations/display.h"
#include "active_house_operations/do_not_disturb.h"
#include "active_house_operations/doorbell_disco.h"

static void handle_touch_command(app_state_t *state,
                                 touch_command_t command)
{
    if (command == TOUCH_SMART_PLUG) {
        app_toggle_smart_plug(state);
    } else if (command == TOUCH_BLINDS) {
        app_cycle_blinds(state);
    } else if (command == TOUCH_RETURN_AUTO) {
        app_return_to_auto(state);
    } else if (command == TOUCH_DND) {
        app_toggle_dnd(state);
    } else if (command == TOUCH_LIGHT_TEST) {
#if LIGHT_SENSOR_SIMULATION == 1
        light_sensor_next_simulated_level();
#endif
    }
}

int main(void)
{
    app_state_t state;
    unsigned long now_ms;
    unsigned long last_display_ms;
    unsigned long demo_minutes;
    unsigned int light_raw;
    unsigned int room_temperature_c;
    touch_command_t touch_command;

    /* Initialize the Lab 6 display before the application GPIO. */
    display_init();
    touch_input_init();

    doorbell_button_init();
    smart_plug_button_init();
    light_sensor_init();
    dial_temp_init();
    audio_init();
    timebase_init();
    blinds_init();
    doorbell_disco_init();

    app_state_init(&state);
    last_display_ms = 0U;
    blinds_show(state.blind_1, state.blind_2);
    display_render(&state);

    while (1) {
        now_ms = timebase_milliseconds();
        demo_minutes = DEMO_START_MINUTES;
        demo_minutes += (now_ms / 1000U) * DEMO_MINUTES_PER_SECOND;
        demo_minutes = demo_minutes % MINUTES_PER_DAY;

        light_raw = light_sensor_read_raw();
        room_temperature_c = dial_temp_read_celsius();
        app_update_time(&state, demo_minutes);
        app_update_sensors(&state, light_raw, room_temperature_c);

        if (smart_plug_button_pressed_edge(now_ms) != 0U) {
            app_toggle_smart_plug(&state);
        }

        if (doorbell_button_pressed_edge(now_ms) != 0U) {
            app_record_doorbell(&state);

            if (do_not_disturb_allows_chime(&state) != 0U) {
                doorbell_disco_play();

                /* Restore the LCD panel and then return SPI0 to touch mode. */
                display_recover_after_disco(&state);
                touch_input_init();
                now_ms = timebase_milliseconds();
                last_display_ms = now_ms;
            }
        }

        touch_command = touch_input_read_command();
        handle_touch_command(&state, touch_command);

        blinds_show(state.blind_1, state.blind_2);

        if (touch_command == TOUCH_RETURN_AUTO) {
            display_render(&state);
            last_display_ms = now_ms;
        }

        if ((now_ms - last_display_ms) >= DISPLAY_REFRESH_MS) {
            display_render(&state);
            last_display_ms = now_ms;
        }
    }
}
