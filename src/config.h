#ifndef DESN2000_CONFIG_H
#define DESN2000_CONFIG_H

/* Raw ADC thresholds. Replace these after a real board calibration. */
#define LIGHT_DARK_MAX_RAW       300U
#define LIGHT_BRIGHT_MIN_RAW     700U

/* The red P0.25 / AD0.2 potentiometer represents room temperature. */
#define ROOM_TEMPERATURE_MIN_C   16U
#define ROOM_TEMPERATURE_MAX_C   30U
#define TARGET_TEMPERATURE_C     22U
#define TEMPERATURE_DEADBAND_C   1U

/* Accelerated demonstration clock. One real second is one demo hour. */
#define DEMO_START_MINUTES       330U
#define DEMO_MINUTES_PER_SECOND  60U
#define MORNING_START_MINUTES    360U
#define DAY_START_MINUTES        720U
#define AFTERNOON_START_MINUTES  900U
#define EVENING_START_MINUTES    1080U
#define NIGHT_START_MINUTES      1320U
#define MINUTES_PER_DAY          1440U

#define DISPLAY_REFRESH_MS       250U
#define BUTTON_DEBOUNCE_MS       100U

/* Touch calibration switches. Change only after observing the board. */
#define TOUCH_PRESSURE_MIN_RAW   16U
#define TOUCH_SWAP_XY            0
#define TOUCH_INVERT_X           0
#define TOUCH_INVERT_Y           0

#endif
