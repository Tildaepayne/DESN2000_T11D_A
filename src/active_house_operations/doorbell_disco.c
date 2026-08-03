#include "doorbell_disco.h"
#include "../lpc24xx.h"
#include "../helper/audio.h"

/* Daughter-board LED ladder: LLAD1-LLAD8 and its active-high enable. */
#define LED_LADDER_ALL       0x000001FE
#define LED_LADDER_ENABLE    0x00400000

/* P2.1-P2.8 share pins with the LCD, so Disco mode owns them temporarily. */
#define PINSEL4_KEEP_OTHERS  0xFFFC0003
#define PINSEL1_KEEP_OTHERS  0xFFFFCFFF
#define LCD_OFF_KEEP_OTHERS  0xFFFFF7FE

#define DISCO_NOTE_RATE_US   52000U
#define DISCO_NOTE_COUNT     58U
#define DISCO_PATTERN_COUNT  16U

#define OFF  100U
#define DS5  1607U
#define E5   1515U
#define FS5  1362U
#define AS5  1073U
#define B5   1012U
#define CS6  902U
#define DS6  803U
#define E6   757U

typedef struct {
    unsigned int duration;
    unsigned int pitch;
    unsigned int volume;
} disco_tone_t;

/* First 58 notes of Lab 5 play_song: the finite Nyan Cat intro. */
static const disco_tone_t disco_song[DISCO_NOTE_COUNT] = {
    {1U, DS5, 768U}, {1U, DS5, 416U},
    {1U, E5, 768U},  {1U, E5, 416U},
    {1U, FS5, 768U}, {1U, FS5, 416U},
    {2U, OFF, 0U},
    {1U, B5, 768U},  {1U, B5, 416U},
    {2U, OFF, 0U},
    {1U, DS5, 768U}, {1U, DS5, 416U},
    {1U, E5, 768U},  {1U, E5, 416U},
    {1U, FS5, 768U}, {1U, FS5, 416U},
    {1U, B5, 768U},  {1U, B5, 416U},
    {1U, CS6, 768U}, {1U, CS6, 416U},
    {1U, DS6, 768U}, {1U, DS6, 416U},
    {1U, CS6, 768U}, {1U, CS6, 416U},
    {1U, AS5, 768U}, {1U, AS5, 416U},
    {1U, B5, 768U},  {1U, B5, 416U},
    {2U, OFF, 0U},
    {1U, FS5, 768U}, {1U, FS5, 416U},
    {2U, OFF, 0U},
    {1U, DS5, 768U}, {1U, DS5, 416U},
    {1U, E5, 768U},  {1U, E5, 416U},
    {1U, FS5, 768U}, {1U, FS5, 416U},
    {2U, OFF, 0U},
    {1U, AS5, 768U}, {1U, AS5, 416U},
    {2U, OFF, 0U},
    {1U, B5, 768U},  {1U, B5, 416U},
    {1U, AS5, 768U}, {1U, AS5, 416U},
    {1U, B5, 768U},  {1U, B5, 416U},
    {1U, CS6, 768U}, {1U, CS6, 416U},
    {1U, E6, 768U},  {1U, E6, 416U},
    {1U, DS6, 768U}, {1U, DS6, 416U},
    {1U, E6, 768U},  {1U, E6, 416U},
    {1U, CS6, 768U}, {1U, CS6, 416U}
};

static const unsigned int disco_patterns[DISCO_PATTERN_COUNT] = {
    0x00000002, 0x00000006, 0x0000000E, 0x0000001C,
    0x00000038, 0x00000070, 0x000000E0, 0x000001C0,
    0x00000180, 0x000001C0, 0x000000E0, 0x00000070,
    0x00000038, 0x0000001C, 0x0000000E, 0x00000006
};

static unsigned int disco_active;
static unsigned int disco_current_phase;
static unsigned long saved_pinsel1;
static unsigned long saved_pinsel4;
static unsigned long saved_fio2dir;
static unsigned long saved_lcd_control;

void doorbell_disco_init(void)
{
    disco_active = 0U;
    disco_current_phase = 0U;

    /* Keep the shared LED ladder disabled during normal LCD operation. */
    PINSEL1 = PINSEL1 & PINSEL1_KEEP_OTHERS;
    IODIR0 = IODIR0 | LED_LADDER_ENABLE;
    IOCLR0 = LED_LADDER_ENABLE;
}

void doorbell_disco_start(void)
{
    if (disco_active != 0U) {
        return;
    }

    saved_pinsel1 = PINSEL1;
    saved_pinsel4 = PINSEL4;
    saved_fio2dir = FIO2DIR;
    saved_lcd_control = LCD_CTRL;

    /* Stop LCD pin activity before changing its shared P2 pins to GPIO. */
    IOCLR0 = LED_LADDER_ENABLE;
    LCD_CTRL = saved_lcd_control & LCD_OFF_KEEP_OTHERS;
    PINSEL1 = saved_pinsel1 & PINSEL1_KEEP_OTHERS;
    PINSEL4 = saved_pinsel4 & PINSEL4_KEEP_OTHERS;

    IODIR0 = IODIR0 | LED_LADDER_ENABLE;
    FIO2DIR = FIO2DIR | LED_LADDER_ALL;
    FIO2CLR = LED_LADDER_ALL;
    IOSET0 = LED_LADDER_ENABLE;

    disco_active = 1U;
    disco_current_phase = 0U;
}

void doorbell_disco_step(void)
{
    if (disco_active == 0U) {
        return;
    }

    FIO2CLR = LED_LADDER_ALL;
    FIO2SET = disco_patterns[disco_current_phase];

    disco_current_phase++;
    if (disco_current_phase >= DISCO_PATTERN_COUNT) {
        disco_current_phase = 0U;
    }
}

void doorbell_disco_stop(void)
{
    if (disco_active == 0U) {
        return;
    }

    IOCLR0 = LED_LADDER_ENABLE;
    FIO2CLR = LED_LADDER_ALL;
    FIO2DIR = saved_fio2dir;
    PINSEL4 = saved_pinsel4;
    PINSEL1 = saved_pinsel1;
    LCD_CTRL = saved_lcd_control;

    disco_active = 0U;
    disco_current_phase = 0U;
}

void doorbell_disco_play(void)
{
    unsigned int note_index;
    unsigned int note_duration_us;

    doorbell_disco_start();

    for (note_index = 0U;
         note_index < DISCO_NOTE_COUNT;
         note_index++) {
        doorbell_disco_step();
        note_duration_us = DISCO_NOTE_RATE_US *
                           disco_song[note_index].duration;
        audio_play_tone(note_duration_us,
                        disco_song[note_index].pitch,
                        disco_song[note_index].volume);
    }

    audio_silence();
    doorbell_disco_stop();
}

unsigned int doorbell_disco_is_active(void)
{
    return disco_active;
}

unsigned int doorbell_disco_phase(void)
{
    return disco_current_phase;
}
