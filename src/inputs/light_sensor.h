#ifndef DESN2000_LIGHT_SENSOR_H
#define DESN2000_LIGHT_SENSOR_H

#include "../app_state.h"

#define LIGHT_SENSOR_ADC_MAX_RAW 1023U

void light_sensor_init(void);
unsigned int light_sensor_read_raw(void);
unsigned int light_sensor_thresholds_valid(unsigned int dark_max_raw,
                                           unsigned int bright_min_raw);
light_level_t light_sensor_classify_raw(unsigned int raw_value,
                                        unsigned int dark_max_raw,
                                        unsigned int bright_min_raw);

#endif
