#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>

#include "shared.h"
#include "dbaccess.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

void InitializeAudio();
void InitializePaths();
void InitializeInstance();
void CleanUpAudio();

char song_1_path[100] = "Audio/Music/Song1.wav";
char current_audio_path[100];



ALLEGRO_DISPLAY *display                 = NULL;
ALLEGRO_SAMPLE *song_1                   = NULL;
ALLEGRO_SAMPLE_INSTANCE *song_1_instance = NULL;


void *AudioEntry(ALLEGRO_THREAD *thread, void *arg) 
{

	InitializeAudio();
    InitializePaths();
    InitializeInstance();

    while (al_get_sample_instance_playing(song_1_instance)) {}
	//al_rest(100);

    return NULL;
}

void InitializeAudio()
{
	al_init();
	al_install_audio();
	al_init_acodec_addon();
	
	al_reserve_samples(1);
}

void InitializePaths()
{
	song_1 = al_load_sample(song_1_path);
}


void InitializeInstance()
{
	song_1_instance = al_create_sample_instance(song_1);
	al_attach_sample_instance_to_mixer(song_1_instance, al_get_default_mixer());
	al_play_sample_instance(song_1_instance);
}

void CleanUpAudio()
{
	al_destroy_sample_instance(song_1_instance);
	al_destroy_sample(song_1);
	al_uninstall_audio();
}