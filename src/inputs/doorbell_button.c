#include "../lpc24xx.h"
#include "../config.h"
#include "doorbell_button.h"

#define DOORBELL_BUTTON 0x00000400

static unsigned int previous_button_state;
static unsigned int accepted_press_exists;
static unsigned long last_accepted_press_ms;

void doorbell_button_init(void)
{
    /* P0.10 is a Fast GPIO input, matching the Lab 4/NyanSim interface. */
    PINSEL0 &= 0xFFCFFFFF;
    FIO0DIR &= 0xFFFFFBFF;
    previous_button_state = doorbell_button_read();
    accepted_press_exists = 0U;
    last_accepted_press_ms = 0U;
}

unsigned int doorbell_button_read(void)
{
    if ((FIO0PIN & DOORBELL_BUTTON) != 0) {
        return 1U;
    }

    return 0U;
}

unsigned int doorbell_button_pressed_edge(unsigned long now_ms)
{
    unsigned int current_button_state;
    unsigned int pressed_edge;

    current_button_state = doorbell_button_read();
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
