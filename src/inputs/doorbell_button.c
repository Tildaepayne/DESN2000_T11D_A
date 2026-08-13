/* Captures debounced P0.10 doorbell presses through the EINT3/VIC IRQ. */
#include "../lpc24xx.h"
#include "../config.h"
#include "doorbell_button.h"

#define DOORBELL_BUTTON          0x00000400
#define EINT3_INTERRUPT          0x00020000
#define EINT3_LOWEST_PRIORITY    15U
#define MICROSECONDS_PER_MS      1000U

static volatile unsigned int press_generation;
static unsigned int consumed_press_generation;
static unsigned int accepted_interrupt_exists;
static unsigned long last_accepted_interrupt_us;

static __irq void doorbell_button_irq_handler(void)
{
    unsigned long now_us;

    if ((IO0_INT_STAT_R & DOORBELL_BUTTON) != 0U) {
        now_us = T0TC;

        if ((accepted_interrupt_exists == 0U) ||
            ((now_us - last_accepted_interrupt_us) >=
             (BUTTON_DEBOUNCE_MS * MICROSECONDS_PER_MS))) {
            press_generation++;
            accepted_interrupt_exists = 1U;
            last_accepted_interrupt_us = now_us;
        }

        IO0_INT_CLR = DOORBELL_BUTTON;
    }

    VICVectAddr = 0U;
}

void doorbell_button_init(void)
{
    /* Disable EINT3 while its GPIO source and vector are configured. */
    VICIntEnClr = EINT3_INTERRUPT;

    /* P0.10 is a Fast GPIO input, matching the board doorbell interface. */
    PINSEL0 &= 0xFFCFFFFF;
    FIO0DIR &= 0xFFFFFBFF;

    /* Use only a rising edge and discard any stale GPIO interrupt status. */
    IO0_INT_EN_R &= ~DOORBELL_BUTTON;
    IO0_INT_EN_F &= ~DOORBELL_BUTTON;
    IO0_INT_CLR = DOORBELL_BUTTON;

    press_generation = 0U;
    consumed_press_generation = 0U;
    accepted_interrupt_exists = 0U;
    last_accepted_interrupt_us = 0U;

    /* Port 0/2 GPIO interrupts share the LPC2478 EINT3 VIC source. */
    VICVectAddr17 = (unsigned long)doorbell_button_irq_handler;
    VICVectPriority17 = EINT3_LOWEST_PRIORITY;
    VICIntSelect &= ~EINT3_INTERRUPT;

    IO0_INT_EN_R |= DOORBELL_BUTTON;
    VICIntEnable = EINT3_INTERRUPT;
}

unsigned int doorbell_button_read(void)
{
    if ((FIO0PIN & DOORBELL_BUTTON) != 0) {
        return 1U;
    }

    return 0U;
}

unsigned int doorbell_button_take_press(void)
{
    unsigned int observed_generation;

    observed_generation = press_generation;
    if (observed_generation == consumed_press_generation) {
        return 0U;
    }

    /* Collapse multiple presses received while the main loop was busy. */
    consumed_press_generation = observed_generation;
    return 1U;
}
