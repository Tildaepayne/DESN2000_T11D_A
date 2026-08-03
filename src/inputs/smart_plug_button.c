#include "../lpc24xx.h"
#include "../config.h"
#include "smart_plug_button.h"

#define SMART_PLUG_BUTTON 0x00000800

static unsigned int previous_button_state;
static unsigned int accepted_press_exists;
static unsigned long last_accepted_press_ms;

void smart_plug_button_init(void)
{
    /* P0.11 is a Fast GPIO input, matching the Lab 4/NyanSim interface. */
    PINSEL0 &= 0xFF3FFFFF;
    FIO0DIR &= 0xFFFFF7FF;
    previous_button_state = smart_plug_button_read();
    accepted_press_exists = 0U;
    last_accepted_press_ms = 0U;
}

unsigned int smart_plug_button_read(void)
{
    if ((FIO0PIN & SMART_PLUG_BUTTON) != 0) {
        return 1U;
    }

    return 0U;
}

unsigned int smart_plug_button_pressed_edge(unsigned long now_ms)
{
    unsigned int current_button_state;
    unsigned int pressed_edge;

    current_button_state = smart_plug_button_read();
    pressed_edge = 0U;

    if ((current_button_state != 0U) &&
        (previous_button_state == 0U)) {
        if ((accepted_press_exists == 0U) ||
            ((now_ms - last_accepted_press_ms) >= BUTTON_DEBOUNCE_MS)) {
            pressed_edge = 1U;
            accepted_press_exists = 1U;
            last_accepted_press_ms = now_ms;
        }
    }

    previous_button_state = current_button_state;
    return pressed_edge;
}
