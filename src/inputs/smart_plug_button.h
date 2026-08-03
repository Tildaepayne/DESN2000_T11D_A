#ifndef DESN2000_SMART_PLUG_BUTTON_H
#define DESN2000_SMART_PLUG_BUTTON_H

void smart_plug_button_init(void);
unsigned int smart_plug_button_read(void);
unsigned int smart_plug_button_pressed_edge(unsigned long now_ms);

#endif
