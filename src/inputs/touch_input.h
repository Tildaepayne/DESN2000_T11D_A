#ifndef DESN2000_TOUCH_INPUT_H
#define DESN2000_TOUCH_INPUT_H

#include "../app_state.h"

void touch_input_init(void);
touch_command_t touch_input_read_command(void);
touch_command_t touch_input_command_from_pixel(unsigned int x,
                                                unsigned int y);

#endif
