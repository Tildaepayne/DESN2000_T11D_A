# DESN2000_T11D_A
The code implementation of DESN2000 T11D A project.
Coded modules: 
- Disco mode
- Morning Routine
  - Expresso Machine
  - Doorbell
- Evening Routine
  - Doorbell
  - Light Sensors
  - Do not disturb
- Temperature display

Files: 
'''
                                         main.c
                                            |
                --------------------------------------------------------
                |                 |                   |                 |       
SEGEMENTS:   morning           evening             disco/            heater/
             routine           routine             doorbell          cooler
                |                 |                    |                |
            -coffee           -idle mode         -music/leds         -leds
            -blind operation  -blind operation   
                 |                      |              |                |
INPUT:     light sensor           light sensor      button            dial

HELPER: 
- LED operation
- blind operation

'''

