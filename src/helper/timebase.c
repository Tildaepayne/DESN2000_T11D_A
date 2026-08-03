#include "../lpc24xx.h"
#include "timebase.h"

void timebase_init(void)
{
    /*
     * Timer 0 is supported by both the QVGA board and NyanSim.
     * Keep it free-running at one count per microsecond so the main clock
     * and the short doorbell delays can safely share the same timer.
     */
    PCONP |= 0x00000002;
    T0TCR = 0x02;
    T0CTCR = 0;
    T0MCR = 0;
    T0PR = (Fpclk / 1000000) - 1;
    T0TCR = 0x01;
}

unsigned long timebase_microseconds(void)
{
    return T0TC;
}

unsigned long timebase_milliseconds(void)
{
    return T0TC / 1000U;
}
