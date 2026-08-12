#ifndef DESN2000_APP_LOGIC_H
#define DESN2000_APP_LOGIC_H

#include "app_state.h"

void app_state_init(app_state_t *state);
void app_update_time(app_state_t *state, unsigned long demo_minutes);
void app_update_sensors(app_state_t *state,
                        unsigned int light_raw,
                        unsigned int room_temperature_c);
void app_apply_automatic_control(app_state_t *state);
void app_toggle_smart_plug(app_state_t *state);
void app_cycle_blinds(app_state_t *state);
void app_toggle_house_lights(app_state_t *state);
void app_return_to_auto(app_state_t *state);
void app_toggle_dnd(app_state_t *state);
void app_record_doorbell(app_state_t *state);

#endif
