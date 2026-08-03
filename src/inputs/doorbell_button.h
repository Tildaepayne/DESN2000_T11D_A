#ifndef DESN2000_DOORBELL_BUTTON_H
#define DESN2000_DOORBELL_BUTTON_H

void doorbell_button_init(void);
unsigned int doorbell_button_read(void);
unsigned int doorbell_button_pressed_edge(unsigned long now_ms);

#endif
