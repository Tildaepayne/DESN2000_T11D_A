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
