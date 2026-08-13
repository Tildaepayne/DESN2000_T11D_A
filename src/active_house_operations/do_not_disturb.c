/* Owns the DND state and doorbell-chime permission decision. */
#include "do_not_disturb.h"

void do_not_disturb_toggle(app_state_t *state)
{
    if (state->dnd_on == 0U) {
        state->dnd_on = 1U;
    } else {
        state->dnd_on = 0U;
    }
}

unsigned int do_not_disturb_allows_chime(const app_state_t *state)
{
    if (state->dnd_on == 0U) {
        return 1U;
    }

    return 0U;
}
