#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "log.h"
#include "audio.h"
#include "shared.h"

void InitializeAllegroAudio();
ALLEGRO_SAMPLE_INSTANCE * CreateSongInstance(ALLEGRO_SAMPLE_INSTANCE *instance_target);
void InitializeSongInstances();
void InitializeAllegroMixer();
void InitializeSampleAudioFiles();
void PlayAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance, bool *take_over);
void StopAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance);
void DestroyAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance);
void CleanUpAudio();
void CleanUpAudioCheckThread();

typedef struct AudioSamples {

    AudioSample sample;
    char *path;
    float gain;
    float pan;
    float speed;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;

} AudioSamples;

static const unsigned char MAX_SAMPLES = 4;
static unsigned char num_samples       = 0;
//static ALLEGRO_SAMPLES *samples        = NULL;

static const unsigned char sample_paths_defined = 1;
static AudioSamples sample_paths[1] = 
{
    {BUTTON_CLICK, "assets/music/click/click.ogg", .05, 0.0, 1.0, NULL}
};
    
void InitializeAudio()
{

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(MAX_SAMPLES);

    for (int i = 0;i < sample_paths_defined;i++) {

        ALLEGRO_SAMPLE *sample          = al_load_sample(sample_paths[i].path);
        sample_paths[i].sample_instance = al_create_sample_instance(sample);
        if (sample_paths[i].sample_instance != NULL) {

            al_set_sample_instance_gain(sample_paths[i].sample_instance, sample_paths[i].gain);
            al_set_sample_instance_pan(sample_paths[i].sample_instance, sample_paths[i].pan);
            al_attach_sample_instance_to_mixer(sample_paths[i].sample_instance, al_get_default_mixer());

        }

    }

}

/*
void InitializeSongInstances()
{

    song_instance = CreateSongInstance(NULL);

}

ALLEGRO_SAMPLE_INSTANCE * CreateSongInstance(ALLEGRO_SAMPLE_INSTANCE *instance_target)
{

    ALLEGRO_SAMPLE_INSTANCE *song_instance = al_create_sample_instance(instance_target);
    al_set_sample_instance_playmode(song_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(song_instance, al_get_default_mixer());

    return song_instance;

}

void PlayAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance, bool *take_over)
{

    if (take_over)
    {
        DestroyAudioInstance(song_instance);  
    }
    current_playing = audio_instance;
    ALLEGRO_SAMPLE_INSTANCE * song_instance = CreateSongInstance(audio_instance);
    al_play_sample_instance(song_instance);

}

void StopAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance)
{

    al_stop_sample_instance(audio_instance);

}

void DestroyAudioInstance(ALLEGRO_SAMPLE_INSTANCE *audio_instance)
{

    al_destroy_sample_instance(audio_instance);

}

void CleanUpAudio()
{

    DestroyAudioInstance(song_instance);

}*/

void PlaySample(AudioSample sample) 
{

    if (num_samples + 1 == MAX_SAMPLES)
        return;

    for (int i = 0; i < sample_paths_defined; i++) {

        if (sample_paths[i].sample == sample) {

            if (sample_paths[i].sample_instance != NULL) {

                al_set_sample_instance_position(sample_paths[i].sample_instance, 0);
                al_play_sample_instance(sample_paths[i].sample_instance);
                //al_play_sample(sample_paths[i].audio_sample, sample_paths[i].gain, sample_paths[i].pan, sample_paths[i].speed, ALLEGRO_PLAYMODE_ONCE, NULL);

            }

            break;

        }

    }

}

void PlayStream(AudioStream stream)
{



}