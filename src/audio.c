#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "shared.h"
#include "dbaccess.h"


void InitializeAllegroAudio();
void CreateSongInstance(ALLEGRO_SAMPLE_INSTANCE *instance_name, ALLEGRO_SAMPLE *instance_target);
void InitializeSongInstances();
void InitializeSampleAudioFiles();
void PlayAudioInstance();
void StopAudioInstance();
void DestroyAudioInstance();
void CleanUpAudio();
void Example();


ALLEGRO_SAMPLE *song_1 = NULL;
ALLEGRO_SAMPLE *song_2 = NULL;

ALLEGRO_SAMPLE_INSTANCE *song_1_instance = NULL;
ALLEGRO_SAMPLE_INSTANCE *song_2_instance = NULL;
	
void *AudioEntry(ALLEGRO_THREAD *thread, void *arg) 
{

	InitializeAllegroAudio();
	InitializeSampleAudioFiles();
    InitializeSongInstances();

    //Example();

    PlayAudioInstance(song_1_instance);
    al_rest(5);
    StopAudioInstance(song_1_instance);
    al_rest(5);
    PlayAudioInstance(song_1_instance);
    PlayAudioInstance(song_2_instance);
    al_rest(8);
    DestroyAudioInstance(song_1_instance);


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
	CreateSongInstance(song_1_instance, song_1);
	
	//song_1_instance = al_create_sample_instance(song_1);
	//al_set_sample_instance_playmode(song_1_instance, ALLEGRO_PLAYMODE_LOOP);
	//al_attach_sample_instance_to_mixer(song_1_instance, al_get_default_mixer());

	song_2_instance = al_create_sample_instance(song_2);
	al_set_sample_instance_playmode(song_2_instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(song_2_instance, al_get_default_mixer());
}

void CreateSongInstance(ALLEGRO_SAMPLE_INSTANCE *instance_name, ALLEGRO_SAMPLE *instance_target)
{
	instance_name = al_create_sample_instance(instance_target);
	al_set_sample_instance_playmode(instance_name, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(instance_name, al_get_default_mixer());
}


void InitializeSampleAudioFiles()
{
	song_1 = al_load_sample( "Audio/Music/Song1.wav" );
	song_2 = al_load_sample( "Audio/Music/Song2.wav" );
}



void PlayAudioInstance(ALLEGRO_SAMPLE *audio_instance)
{	
	al_play_sample_instance(audio_instance);
}

void StopAudioInstance(ALLEGRO_SAMPLE *audio_instance)
{
	al_stop_sample_instance(audio_instance);
}

void DestroyAudioInstance(ALLEGRO_SAMPLE *audio_instance)
{
	al_destroy_sample_instance(audio_instance);
}

void Example()
{

}


void CleanUpAudio()
{
	//DestroyAudioInstance(song_1_instance);
	//DestroyAudioInstance(song_2_instance);
}