#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

#include <allegro5/allegro_audio.h>

typedef enum AudioStream {

    DEFAULT

} AudioStream;

void audio_init();
uint32_t audio_get_sample_id(char *sample_name);
void audio_play_sample(uint32_t sample_id);
void audio_play_stream(AudioStream stream);

#endif