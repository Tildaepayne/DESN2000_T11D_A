#include "heating_cooling.h"

heating_mode_t heating_cooling_choose_mode(unsigned int room_temperature_c,
                                           unsigned int target_temperature_c,
                                           unsigned int deadband_c)
{
    if ((room_temperature_c + deadband_c) < target_temperature_c) {
        return HVAC_HEATING;
    }

    if (room_temperature_c > (target_temperature_c + deadband_c)) {
        return HVAC_COOLING;
    }

    return HVAC_COMFORTABLE;
}
