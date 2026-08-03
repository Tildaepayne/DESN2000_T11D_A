#ifndef DESN2000_DOORBELL_DISCO_H
#define DESN2000_DOORBELL_DISCO_H

void doorbell_disco_init(void);
void doorbell_disco_play(void);
void doorbell_disco_start(void);
void doorbell_disco_step(void);
void doorbell_disco_stop(void);
unsigned int doorbell_disco_is_active(void);
unsigned int doorbell_disco_phase(void);

#endif
