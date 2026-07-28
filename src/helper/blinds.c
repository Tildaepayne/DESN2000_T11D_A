////////////////////////////////////////////////////////////////////////////////
/*
COURSE: DESN2000
GROUP: T11D_A
FUNCTION: blinds.c
PURPOSE: This file implements the blinds control functionality.
DEPENDENTS:
    (light_sensor.c)
           |
        - blinds.c
                |
            - evening_routine.c
            - morning_routine.c
                         |
                       display.c
*/
////////////////////////////////////////////////////////////////////////////////

#include "../lpc24xx.h"
#include "../main.h"
