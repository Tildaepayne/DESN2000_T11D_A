#ifndef DESN2000_DISPLAY_H
#define DESN2000_DISPLAY_H

#include "../app_state.h"

void display_init(void);
void display_render(const app_state_t *state);
void display_recover_after_disco(const app_state_t *state);

#endif
