#ifndef DESN2000_DO_NOT_DISTURB_H
#define DESN2000_DO_NOT_DISTURB_H

#include "../app_state.h"

void do_not_disturb_toggle(app_state_t *state);
unsigned int do_not_disturb_allows_chime(const app_state_t *state);

#endif
