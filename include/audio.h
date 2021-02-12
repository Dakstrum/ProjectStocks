#ifndef AUDIO_H
#define AUDIO_H

#include <allegro5/allegro_audio.h>

typedef enum AudioSample {

    BUTTON_CLICK,
    FLIPCARD

} AudioSample;

typedef enum AudioStream {

    DEFAULT

} AudioStream;

void InitializeAudio();
void PlaySample(AudioSample sample);
void PlayStream(AudioStream stream);

#endif