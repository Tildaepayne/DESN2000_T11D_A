#ifndef DESN2000_CONFIG_H
#define DESN2000_CONFIG_H

/*
 * Use the real AD0.1 driver. NyanSim supplies its light control through the
 * same ADC channel, so this path tests the same call used by the real board.
 * Change this to 1 only when an injected software value is required.
 */
#ifndef LIGHT_SENSOR_SIMULATION
#define LIGHT_SENSOR_SIMULATION 0
#endif

/* Raw ADC thresholds. Replace these after a real board calibration. */
#define LIGHT_DARK_MAX_RAW       300U
#define LIGHT_BRIGHT_MIN_RAW     700U
#define LIGHT_SIMULATED_DARK     150U
#define LIGHT_SIMULATED_MEDIUM   500U
#define LIGHT_SIMULATED_BRIGHT   850U

/* The onboard P0.25 / AD0.2 dial represents room temperature. */
#define ROOM_TEMPERATURE_MIN_C   16U
#define ROOM_TEMPERATURE_MAX_C   30U
#define TARGET_TEMPERATURE_C     22U
#define TEMPERATURE_DEADBAND_C   1U

/* Accelerated demonstration clock. One real second is one demo hour. */
#define DEMO_START_MINUTES       330U
#define DEMO_MINUTES_PER_SECOND  60U
#define MORNING_START_MINUTES    360U
#define DAY_START_MINUTES        720U
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
