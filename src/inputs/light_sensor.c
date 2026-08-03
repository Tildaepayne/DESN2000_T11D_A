#include "../lpc24xx.h"
#include "../config.h"
#include "light_sensor.h"

#define LIGHT_ADC_POLL_LIMIT 100000U

static unsigned int simulated_raw_value = LIGHT_SIMULATED_MEDIUM;
#if LIGHT_SENSOR_SIMULATION == 0
static unsigned int previous_real_raw_value;
#endif

void light_sensor_init(void)
{
#if LIGHT_SENSOR_SIMULATION == 0
    /* Power the ADC. */
    PCONP |= 0x00001000;

    /* Select AD0.1 on P0.24. */
    PINSEL1 &= 0xFFFCFFFF;
    PINSEL1 |= 0x00010000;
    PINMODE1 &= 0xFFFCFFFF;
    PINMODE1 |= 0x00020000;

    /* Polling is used, so ADC interrupts remain disabled. */
    AD0INTEN = 0;
    AD0CR = 0x00200302;
#endif
}

unsigned int light_sensor_read_raw(void)
{
#if LIGHT_SENSOR_SIMULATION == 1
    return simulated_raw_value;
#else
    unsigned long result;
    unsigned int poll_count;

    /* Select channel 1, power the ADC and start one conversion. */
    AD0CR = 0x00200302;
    AD0CR |= 0x01000000;
    poll_count = 0U;

    do {
        result = AD0DR1;
        poll_count++;
    } while (((result & 0x80000000) == 0) &&
             (poll_count < LIGHT_ADC_POLL_LIMIT));

    AD0CR &= 0xF8FFFFFF;

    if ((result & 0x80000000) == 0) {
        return previous_real_raw_value;
    }

    result = result >> 6;
    result = result & 0x000003FF;
    previous_real_raw_value = (unsigned int)result;
    return previous_real_raw_value;
#endif
}

void light_sensor_set_simulated_raw(unsigned int raw_value)
{
    if (raw_value <= LIGHT_SENSOR_ADC_MAX_RAW) {
        simulated_raw_value = raw_value;
    }
}

void light_sensor_next_simulated_level(void)
{
    if (simulated_raw_value == LIGHT_SIMULATED_DARK) {
        simulated_raw_value = LIGHT_SIMULATED_MEDIUM;
    } else if (simulated_raw_value == LIGHT_SIMULATED_MEDIUM) {
        simulated_raw_value = LIGHT_SIMULATED_BRIGHT;
    } else {
        simulated_raw_value = LIGHT_SIMULATED_DARK;
    }
}

unsigned int light_sensor_thresholds_valid(unsigned int dark_max_raw,
                                           unsigned int bright_min_raw)
{
    if (dark_max_raw >= bright_min_raw) {
        return 0U;
    }

    if (bright_min_raw > LIGHT_SENSOR_ADC_MAX_RAW) {
        return 0U;
    }

    return 1U;
}

light_level_t light_sensor_classify_raw(unsigned int raw_value,
                                        unsigned int dark_max_raw,
                                        unsigned int bright_min_raw)
{
    if (raw_value > LIGHT_SENSOR_ADC_MAX_RAW) {
        return LIGHT_LEVEL_MEDIUM;
    }

    if (light_sensor_thresholds_valid(dark_max_raw,
                                      bright_min_raw) == 0U) {
        return LIGHT_LEVEL_MEDIUM;
    }

    if (raw_value <= dark_max_raw) {
        return LIGHT_LEVEL_DARK;
    }

    if (raw_value >= bright_min_raw) {
        return LIGHT_LEVEL_BRIGHT;
    }

    return LIGHT_LEVEL_MEDIUM;
}
