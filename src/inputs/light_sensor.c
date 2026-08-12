////////////////////////////////////////////////////////////////////////////////
/*
COURSE: DESN2000
GROUP: T11D_A
FUNCTION: light_sensor.c
PURPOSE: This file contains the implementation of the light sensor input 
        functionality.
DEPENDENTS:
    - light_sensor.c
            |
        - blinds.c
                |
            - evening_routine.c
            - morning_routine.c
                         |
                       display.c
LUX_SCALE:
    - 0-50: triggers do_not_disturb
    - 51-300: triggers morning_routine or evening_routine
    - >300: triggers half-blind
*/
////////////////////////////////////////////////////////////////////////////////

#include "lpc24xx.h"

#define PCONP_PCADC        (1 << 12)
#define PINSEL1_AD01_MASK  (3 << 16)
#define PINSEL1_AD01_SEL   (1 << 16)
#define PINMODE1_AD01_MASK (3 << 16)
#define PINMODE1_AD01_OFF  (2 << 16)
#define AD0CR_SEL_CH1      (1 << 1)
#define AD0CR_CLKDIV       (5 << 8)
#define AD0CR_PDN          (1 << 21)
#define AD0CR_START_MASK   (7 << 24)
#define AD0CR_START_NOW    (1 << 24)
#define AD0DR_DONE         0x80000000

#define LUX_FULL_SCALE     660
#define LS_AVG_SAMPLES     8

void light_sensor_init(void)
{
    PCONP |= PCONP_PCADC;

    PINSEL1 &= ~PINSEL1_AD01_MASK;
    PINSEL1 |= PINSEL1_AD01_SEL;

    PINMODE1 &= ~PINMODE1_AD01_MASK;
    PINMODE1 |= PINMODE1_AD01_OFF;

    AD0CR = AD0CR_SEL_CH1 | AD0CR_CLKDIV | AD0CR_PDN;
}

unsigned int light_sensor_read_raw(void)
{
    unsigned int result;

    AD0CR &= ~AD0CR_START_MASK;
    AD0CR |= AD0CR_START_NOW;

    result = AD0DR1;
    while ((result & AD0DR_DONE) == 0)
    {
        result = AD0DR1;
    }

    AD0CR &= ~AD0CR_START_MASK;

    return (result >> 6) & 0x3FF; 
}

unsigned int light_sensor_read_lux(void)
{
    unsigned int   sum;
    unsigned int i;
    unsigned int raw;

    sum = 0;
    for (i = 0; i < LS_AVG_SAMPLES; i++)
    {
        sum += light_sensor_read_raw();
    }
    raw = sum / LS_AVG_SAMPLES;

    return (raw * LUX_FULL_SCALE) >> 10;
}

// call this if you want light sensor band
unsigned int light_sensor_get_band(void)
{
    unsigned int lux;
    unsigned int band;

    lux = light_sensor_read_lux();

    if (lux <= 50)
    {
        band = 0;
    }
    else if (lux <= 300)
    {
        band = 1;
    }
    else
    {
        band = 2;
    }

    return band;
}  
