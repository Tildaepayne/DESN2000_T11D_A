#include "../lpc24xx.h"
#include "../config.h"
#include "dial_temp.h"

#define DIAL_ADC_POLL_LIMIT 100000U
#define DIAL_SAFE_START_RAW 438U

static unsigned int previous_dial_raw = DIAL_SAFE_START_RAW;

void dial_temp_init(void)
{
    /* The onboard potentiometer is connected to P0.25 / AD0.2. */
    PCONP |= 0x00001000;
    PINSEL1 &= 0xFFF3FFFF;
    PINSEL1 |= 0x00040000;
    PINMODE1 &= 0xFFF3FFFF;
    PINMODE1 |= 0x00080000;
    AD0INTEN = 0;
}

unsigned int dial_temp_read_raw(void)
{
    unsigned long result;
    unsigned int poll_count;

    AD0CR = 0x00200304;
    AD0CR |= 0x01000000;
    poll_count = 0U;

    do {
        result = AD0DR2;
        poll_count++;
    } while (((result & 0x80000000) == 0) &&
             (poll_count < DIAL_ADC_POLL_LIMIT));

    AD0CR &= 0xF8FFFFFF;

    if ((result & 0x80000000) == 0) {
        return previous_dial_raw;
    }

    result = result >> 6;
    result = result & 0x000003FF;
    previous_dial_raw = (unsigned int)result;
    return previous_dial_raw;
}

unsigned int dial_temp_read_celsius(void)
{
    unsigned int raw_value;
    unsigned int temperature_range;

    raw_value = dial_temp_read_raw();
    temperature_range = ROOM_TEMPERATURE_MAX_C - ROOM_TEMPERATURE_MIN_C;

    return ROOM_TEMPERATURE_MIN_C +
           ((raw_value * temperature_range) / 1023U);
}
