#include "evening_routine.h"

void evening_routine_apply(app_state_t *state)
{
    if (state->smart_plug_manual == 0U) {
        state->smart_plug_on = 0U;
    }

    if (state->blinds_manual == 0U) {
        state->blind_1 = BLIND_DOWN;
        state->blind_2 = BLIND_DOWN;
    }

    state->house_lights_on = 1U;
}
