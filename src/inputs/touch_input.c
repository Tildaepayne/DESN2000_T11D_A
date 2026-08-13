/* Reads TSC2046 touches over SPI0 and maps them to application commands. */
#include "../lpc24xx.h"
#include "../config.h"
#include "touch_input.h"

#define TOUCH_CS 0x00100000
#define TOUCH_READ_X 0x00D8
#define TOUCH_READ_Y 0x0098
#define TOUCH_READ_Z1 0x00B8
#define TOUCH_READ_Z2 0x00C8
#define TOUCH_SPI_FAILSAFE_LIMIT 5000U

static unsigned int touch_is_held;

static unsigned int touch_transfer(unsigned int value)
{
    unsigned int failsafe;

    S0SPDR = value;
    failsafe = 0U;

    while (((S0SPSR & 0x80) == 0) &&
           (failsafe < TOUCH_SPI_FAILSAFE_LIMIT)) {
        /* Wait for the SPI transfer to finish. */
        failsafe++;
    }

    if (failsafe >= TOUCH_SPI_FAILSAFE_LIMIT) {
        return 0U;
    }

    return S0SPDR;
}

static unsigned int touch_read_value(unsigned int command)
{
    unsigned int result;

    FIO0CLR = TOUCH_CS;
    touch_transfer(command);
    result = touch_transfer(0x0000);
    FIO0SET = TOUCH_CS;

    return result & 0x00FF;
}

void touch_input_init(void)
{
    /* P0.15=SCK, P0.17=MISO, P0.18=MOSI. */
    PINSEL0 &= 0x3FFFFFFF;
    PINSEL0 |= 0xC0000000;
    PINSEL1 &= 0xFFFFFFC3;
    PINSEL1 |= 0x0000003C;

    /* P0.20 is the active-low touchscreen chip select. */
    PINSEL1 &= 0xFFFFFCFF;
    FIO0DIR |= TOUCH_CS;
    FIO0SET = TOUCH_CS;

    /* 9-bit, master, CPHA=1, CPOL=1, MSB first, no interrupt. */
    S0SPCCR = 0x24;
    S0SPCR = 0x093C;
    touch_is_held = 0U;
}

touch_command_t touch_input_command_from_pixel(unsigned int x,
                                                unsigned int y)
{
    if ((y >= 250U) && (y <= 282U)) {
        if (x < 80U) {
            return TOUCH_SMART_PLUG;
        }

        if (x < 160U) {
            return TOUCH_BLINDS;
        }

        return TOUCH_RETURN_AUTO;
    }

    if (y >= 285U) {
        if (x < 120U) {
            return TOUCH_DND;
        }

        return TOUCH_HOUSE_LIGHTS;
    }

    return TOUCH_NONE;
}

touch_command_t touch_input_read_command(void)
{
    unsigned int raw_pressure;
    unsigned int raw_z1;
    unsigned int raw_z2;
    unsigned int raw_x;
    unsigned int raw_y;
    unsigned int pixel_x;
    unsigned int pixel_y;
#if TOUCH_SWAP_XY == 1
    unsigned int temporary;
#endif

    /* Lab 6 relative-pressure calculation using both TSC2046 channels. */
    raw_z1 = touch_read_value(TOUCH_READ_Z1);
    raw_z2 = touch_read_value(TOUCH_READ_Z2);
    raw_pressure = raw_z1 + 255U - raw_z2;

    if (raw_pressure < TOUCH_PRESSURE_MIN_RAW) {
        touch_is_held = 0U;
        return TOUCH_NONE;
    }

    raw_x = touch_read_value(TOUCH_READ_X);
    raw_y = touch_read_value(TOUCH_READ_Y);

    if ((raw_x < 8U) || (raw_y < 8U) ||
        (raw_x > 247U) || (raw_y > 247U)) {
        touch_is_held = 0U;
        return TOUCH_NONE;
    }

    if (touch_is_held != 0U) {
        return TOUCH_NONE;
    }

    touch_is_held = 1U;

#if TOUCH_SWAP_XY == 1
    temporary = raw_x;
    raw_x = raw_y;
    raw_y = temporary;
#endif

#if TOUCH_INVERT_X == 1
    raw_x = 255U - raw_x;
#endif

#if TOUCH_INVERT_Y == 1
    raw_y = 255U - raw_y;
#endif

    pixel_x = (raw_x * 239U) / 255U;
    pixel_y = (raw_y * 319U) / 255U;

    return touch_input_command_from_pixel(pixel_x, pixel_y);
}
