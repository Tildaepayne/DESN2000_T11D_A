#ifndef DESN2000_HEATING_COOLING_H
#define DESN2000_HEATING_COOLING_H

#include "../app_state.h"

heating_mode_t heating_cooling_choose_mode(unsigned int room_temperature_c,
                                           unsigned int target_temperature_c,
                                           unsigned int deadband_c);

#endif
