/* Generates blocking DAC tones using the shared Timer0 timebase. */
#include "../lpc24xx.h"
#include "audio.h"
#include "timebase.h"

static void audio_delay_us(unsigned int delay_in_us)
{
    unsigned long start_time_us;

    if (delay_in_us == 0U) {
        return;
    }

    start_time_us = timebase_microseconds();

    while ((timebase_microseconds() - start_time_us) < delay_in_us) {
        /* Poll the shared free-running Timer 0. */
    }
}

void audio_play_tone(unsigned int duration_us,
                     unsigned int period_us,
                     unsigned int volume)
{
    unsigned int elapsed_us;
    unsigned int half_period_us;
    unsigned int dac_value;

    if (duration_us == 0U) {
        DACR = 0;
        return;
    }

    if ((period_us == 0U) || (volume == 0U)) {
        DACR = 0;
        audio_delay_us(duration_us);
        return;
    }

    if (volume > 1023U) {
        volume = 1023U;
    }

    elapsed_us = 0U;
    half_period_us = period_us / 2U;
    dac_value = volume * 64U;

    while (elapsed_us < duration_us) {
        DACR = dac_value;
        audio_delay_us(half_period_us);
        DACR = 0;
        audio_delay_us(half_period_us);
        elapsed_us += period_us;
    }

    DACR = 0;
}

void audio_init(void)
{
    /* Select AOUT function 2 on P0.26. */
    PINSEL1 &= 0xFFCFFFFF;
    PINSEL1 |= 0x00200000;
    DACR = 0;
}

void audio_play_short_chime(void)
{
    audio_play_tone(90000U, 1000U, 768U);
    audio_delay_us(20000U);
    audio_play_tone(120000U, 750U, 768U);
    DACR = 0;
}

void audio_silence(void)
{
    DACR = 0;
}
