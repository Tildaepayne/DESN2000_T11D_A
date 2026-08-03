#ifndef DESN2000_DIAL_TEMP_H
#define DESN2000_DIAL_TEMP_H

void dial_temp_init(void);
unsigned int dial_temp_read_raw(void);
unsigned int dial_temp_read_celsius(void);

#endif
