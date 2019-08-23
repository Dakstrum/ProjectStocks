#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "shared.h"
#include "dbaccess.h"

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

ALLEGRO_SAMPLE_INSTANCE *song_instance  = NULL;
ALLEGRO_SAMPLE_INSTANCE *current_playing = NULL;
    
void *AudioEntry(ALLEGRO_THREAD *thread, void *arg) 
{
    InitializeAllegroAudio();
    InitializeSongInstances();

    return NULL;
}

void InitializeAllegroAudio()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);
}


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

}