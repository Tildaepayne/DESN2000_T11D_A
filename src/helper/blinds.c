#include "../lpc24xx.h"
#include "blinds.h"

#define BLINDS_ALL_RGB  0x003F0000

#define BLIND_1_RED     0x00010000
#define BLIND_1_GREEN   0x00020000
#define BLIND_1_BLUE    0x00040000

#define BLIND_2_RED     0x00080000
#define BLIND_2_GREEN   0x00100000
#define BLIND_2_BLUE    0x00200000

static unsigned long blind_1_colour(blind_position_t position)
{
    if (position == BLIND_UP) {
        return BLIND_1_RED;
    }

    if (position == BLIND_DOWN) {
        return BLIND_1_BLUE;
    }

    return BLIND_1_GREEN;
}

static unsigned long blind_2_colour(blind_position_t position)
{
    if (position == BLIND_UP) {
        return BLIND_2_RED;
    }

    if (position == BLIND_DOWN) {
        return BLIND_2_BLUE;
    }

    return BLIND_2_GREEN;
}

void blinds_init(void)
{
    /* P3.16-P3.21 are the two blind RGB indicators from the brief. */
    PINSEL7 &= 0xFFFFF000;
    FIO3DIR |= BLINDS_ALL_RGB;
    FIO3CLR = BLINDS_ALL_RGB;
}

blind_position_t blinds_position_for_light(light_level_t light_level)
{
    if (light_level == LIGHT_LEVEL_DARK) {
        return BLIND_UP;
    }

    if (light_level == LIGHT_LEVEL_BRIGHT) {
        return BLIND_DOWN;
    }

    return BLIND_MID;
}

blind_position_t blinds_next_position(blind_position_t current_position)
{
    if (current_position == BLIND_UP) {
        return BLIND_MID;
    }

    if (current_position == BLIND_MID) {
        return BLIND_DOWN;
    }

    return BLIND_UP;
}

void blinds_show(blind_position_t blind_1,
                 blind_position_t blind_2)
{
    FIO3CLR = BLINDS_ALL_RGB;
    FIO3SET = blind_1_colour(blind_1);
    FIO3SET = blind_2_colour(blind_2);
}
