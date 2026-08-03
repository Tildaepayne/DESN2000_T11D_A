#ifndef DESN2000_BLINDS_H
#define DESN2000_BLINDS_H

#include "../app_state.h"

void blinds_init(void);
blind_position_t blinds_position_for_light(light_level_t light_level);
blind_position_t blinds_next_position(blind_position_t current_position);
void blinds_show(blind_position_t blind_1,
                 blind_position_t blind_2);

#endif
