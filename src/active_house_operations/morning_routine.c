#include "morning_routine.h"

void morning_routine_apply(app_state_t *state)
{
    if (state->smart_plug_manual == 0U) {
        state->smart_plug_on = 1U;
    }

    if (state->house_lights_manual == 0U) {
        state->house_lights_on = 0U;
    }
}
