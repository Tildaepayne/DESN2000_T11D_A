DESN2000 T11D A - SMART COTTAGE HUB
===================================

1. PROJECT PURPOSE
------------------

This project is a local, wired smart-cottage prototype running on an LPC2478
ARM7TDMI-S, QVGA Base Board and Daughter Board. It demonstrates automated
blinds, an espresso-machine smart plug, house lights, heating/cooling status,
an interrupt-driven doorbell, a doorbell song with an LED Disco effect, and Do
Not Disturb with a missed-guest counter.

No 240 V appliance is controlled. The LCD, RGB indicators, QVGA LEDs and other
low-voltage board outputs safely represent the cottage devices.


2. SOURCE STATUS
----------------

This README documents only the production source tree under src.

The current source compiles and links with 0 errors and 0 warnings for the
QVGA Base Board target. Build success does not prove physical hardware
behaviour. TEMT6000 calibration, calibrated touch-control behaviour, GPIO
interrupt behaviour, LEDs, audio and LCD recovery still require final board
tests.


3. SYSTEM ARCHITECTURE
----------------------

The application is a single-threaded superloop with one short hardware
interrupt. There is no RTOS and no dynamic memory.

The superloop reads Timer0, the AD0.1 light sensor, the AD0.2 red temperature
potentiometer, the P0.11 smart-plug button and the SPI0 touchscreen. It updates
one shared app_state_t structure, applies the automation rules, drives the
outputs and refreshes the LCD every 250 ms.

P0.10 is the only application interrupt. Its rising edge is routed through the
LPC2478 EINT3/VIC source 17. The interrupt service routine uses Timer0 for a
100 ms debounce interval, latches a doorbell event, clears the GPIO interrupt
and acknowledges the VIC. The main loop performs the long work: updating the
state, applying Do Not Disturb, playing audio and Disco, and recovering the LCD.

Timer0 is a free-running 1 us counter shared by the demonstration clock,
button debounce and DAC audio timing. ADC, SPI, I2C and the P0.11 button are
polled. Poll limits prevent unsupported or unavailable peripherals from
blocking the application forever.


4. SYSTEM FEATURES
------------------

Automated blinds

- Blind 1 represents the north-facing window. During 12:00-14:59 it responds
  to the ambient-light classification while Blind 2 remains up.
- Blind 2 represents the west-facing window. During 15:00-17:59 it responds
  to ambient light while Blind 1 remains up.
- During Evening and Night both blinds are down.
- Each blind uses an RGB indicator: red is Up, green is Mid-way and blue is
  Down.
- The BLIND touchscreen button manually cycles both blinds through Up,
  Mid-way and Down.

Light sensor

- The TEMT6000 is read from P0.24/AD0.1 as a 10-bit value from 0 to 1023.
- Values from 0 to 300 are Dark, 301 to 699 are Medium, and 700 to 1023 are
  Bright.
- The thresholds are demonstration values and require physical calibration.
- The integrated application has no injected light value or software light
  simulator.

Temperature and HVAC display

- The red potentiometer on the QVGA Base Board simulates room temperature.
- P0.25/AD0.2 is mapped to 16 to 30 degrees C.
- The target is 22 degrees C with a 1 degree C deadband.
- 16 to 20 degrees C displays Heating, 21 to 23 displays Comfortable, and
  24 to 30 displays Cooling.
- This is an LCD status simulation; no heater or cooler is driven.

Smart plug

- The espresso-machine smart plug is on during the Morning routine and off at
  other times while it is in automatic mode.
- P0.11/S2 or the PLUG touchscreen button toggles it manually.

House lights

- The HOUSE LIGHT touchscreen button toggles the first four red LEDs on the
  QVGA Base Board.
- I2C0 sends the state to the PCA9532. LEDs 5-8 are not changed.
- Automatic mode turns the house lights on during Evening/Night and off during
  Morning/Day.
- House-light control is independent of the AD0.1 light-sensor reading.

Doorbell, Disco and Do Not Disturb

- P0.10/S1 generates the interrupt-driven doorbell event.
- With Do Not Disturb off, a press plays the DAC song and Daughter Board LED
  ladder pattern.
- With Do Not Disturb on, the song and LEDs are suppressed and the missed-guest
  counter increases.
- P2.1-P2.8 are shared by the LCD and LED ladder. Disco temporarily saves the
  LCD configuration, uses the pins as GPIO, then restores and redraws the LCD.

Touchscreen and LCD

- The display is 240 by 320 pixels using 16-bit RGB565 pixels in external SDRAM
  at address 0xA0000000.
- The TSC2046 touchscreen is read through polling SPI0.
- Raw X/Y values are clamped and scaled with the physical-panel limits in
  src/config.h. The current board was measured as X 8-238 and Y 38-239.
- The controls are PLUG, BLIND, AUTO RESET, DND and HOUSE LIGHT.
- A held touch generates only one command; the screen must be released before
  the same control can trigger again.


5. MANUAL OVERRIDES AND AUTO RESET
----------------------------------

Manual control has priority over the automatic routine. Manually changing the
smart plug, blinds or house lights disables routine control for that feature
only. Its selected state is retained while time and sensor updates continue to
control all other features.

For example, manually changing the blinds pauses only automatic blind movement;
the smart plug and house lights can still follow the current routine.

AUTO RESET clears all manual-override flags, clears Do Not Disturb and the
event counters, and immediately recalculates the outputs from the current time
and sensor readings.


6. AUTOMATIC ROUTINES
---------------------

- 00:00-05:59, Night: smart plug off, both blinds down, house lights on.
- 06:00-11:59, Morning: smart plug on, both blinds up, house lights off.
- 12:00-14:59, Day: smart plug off, Blind 1 follows light, Blind 2 up, house
  lights off.
- 15:00-17:59, Day: smart plug off, Blind 1 up, Blind 2 follows light, house
  lights off.
- 18:00-21:59, Evening: smart plug off, both blinds down, house lights on.
- 22:00-23:59, Night: smart plug off, both blinds down, house lights on.

The demonstration starts at 05:30. One real second advances the demonstration
clock by one hour.


7. HARDWARE CONNECTIONS
-----------------------

- P0.10/S1: active-high doorbell; rising-edge GPIO interrupt through EINT3/VIC.
- P0.11/S2: polled smart-plug manual override.
- P0.24/AD0.1: TEMT6000 ambient-light input.
- P0.25/AD0.2: red potentiometer used to simulate room temperature.
- P0.26/AOUT: DAC speaker output.
- P0.27/SDA0 and P0.28/SCL0: PCA9532 control for house-light LEDs 1-4.
- P0.20: active-low TSC2046 touchscreen chip select.
- P0.15, P0.17 and P0.18: SPI0 touchscreen signals.
- P0.22: active-high Daughter Board LED-ladder enable.
- P2.1-P2.8: LCD interface and temporary Disco LED-ladder output.
- P3.16-P3.18: Blind 1 red, green and blue outputs.
- P3.19-P3.21: Blind 2 red, green and blue outputs.
- External SDRAM: LCD framebuffer at 0xA0000000.

For a physical TEMT6000 test, connect the sensor circuit to 3.3 V,
P0.24/AD0.1 and ground using the supplied 10 kohm arrangement. Disconnect the
J12 accelerometer-Y route from P0.24 first.

For the red temperature potentiometer, confirm J11 selects the potentiometer
and J12 does not also connect the accelerometer-X signal to P0.25.


8. WHERE EACH FEATURE IS IMPLEMENTED
------------------------------------

Source folder layout

- src/inputs contains the ADC, GPIO-button and touchscreen input drivers.
- src/helper contains shared Timer0, DAC-audio and blind RGB support.
- src/active_house_operations contains automation outputs, routines, DND,
  Doorbell Disco and the project-specific LCD dashboard.
- src/lcd contains the reused Lab 6 LCD, graphics, font and SDRAM support.
- A header file beside a module declares its public types and functions; the
  matching C file contains the implementation.

Application coordination

- src/main.c initializes all peripherals, owns the superloop, dispatches touch
  commands, consumes doorbell events and writes application state to outputs.
- src/app_state.h defines the shared state, modes and touch command values.
- src/app_logic.c selects routines, processes sensor readings, applies automatic
  rules and implements all manual overrides and AUTO RESET.
- src/config.h contains light thresholds, temperature range, routine times,
  debounce interval, display refresh interval and touch calibration settings.
- src/ui_layout.h defines the button rectangles shared by LCD drawing and touch
  hit-testing, including the complete HOUSE LIGHT shape.

Inputs

- src/inputs/light_sensor.c and light_sensor.h implement hardware-only AD0.1
  sampling, timeout fallback, threshold validation and light classification.
- src/inputs/dial_temp.c and dial_temp.h read AD0.2 and map the red
  potentiometer to the simulated room-temperature range.
- src/inputs/doorbell_button.c and doorbell_button.h configure the P0.10 rising
  edge, EINT3/VIC handler, Timer0 debounce and main-loop event handoff.
- src/inputs/smart_plug_button.c and smart_plug_button.h implement P0.11 polling,
  rising-edge detection and debounce.
- src/inputs/touch_input.c and touch_input.h configure SPI0/TSC2046, reject
  invalid touches, convert coordinates and map the five control regions.

Automation and outputs

- src/active_house_operations/morning_routine.c applies Morning smart-plug and
  house-light defaults unless those features are in manual mode.
- src/active_house_operations/evening_routine.c applies Evening smart-plug,
  blind and house-light defaults unless those features are in manual mode.
- src/active_house_operations/heating_cooling.c selects Heating, Comfortable or
  Cooling from the simulated temperature and deadband.
- src/helper/blinds.c drives both RGB indicators and converts light level to a
  blind position.
- src/active_house_operations/house_lights.c owns I2C0/PCA9532 and LEDs 1-4.
- src/active_house_operations/do_not_disturb.c owns the DND toggle and chime
  permission decision.
- src/active_house_operations/doorbell_disco.c owns the song sequence, LED
  ladder pattern and temporary LCD-pin handover.
- src/helper/audio.c produces DAC tones using Timer0 polling.
- src/helper/timebase.c configures the shared free-running Timer0 counter.

User interface and platform support

- src/active_house_operations/display.c draws the fixed dashboard, status fields
  and five touchscreen controls, and redraws the display after Disco.
- src/lcd contains the reused Lab 6 LCD controller, graphics, font and SDRAM
  support. These files provide the framebuffer and drawing primitives; the
  project-specific layout remains in display.c.
- src/LPC2400.s is the reused LPC2478 startup and IRQ dispatch code.
- src/lpc24xx.h defines the LPC2478 memory-mapped peripheral registers used by
  the application.
- src/delay.c and delay.h provide the reused blocking delay required by LCD and
  SDRAM initialization.
