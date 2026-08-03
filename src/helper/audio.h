#ifndef DESN2000_AUDIO_H
#define DESN2000_AUDIO_H

void audio_init(void);
void audio_play_tone(unsigned int duration_us,
                     unsigned int period_us,
                     unsigned int volume);
void audio_play_short_chime(void);
void audio_silence(void);

#endif
