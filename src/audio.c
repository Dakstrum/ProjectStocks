#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "log.h"
#include "audio.h"
#include "shared.h"

#include "dbaudio.h"
#include "vector.h"


Vector *audio_samples = NULL;
Vector *audio_sample_instances = NULL;
ALLEGRO_SAMPLE_INSTANCE *sample_instance;

static const unsigned char MAX_SAMPLES = 4;

void audio_load_sample(AudioSample sample) 
{

    ALLEGRO_SAMPLE *temp_sample = al_load_sample(sample.sample_path);
    Vector_PushBackPtr(audio_sample_instances, al_create_sample_instance(temp_sample));

    ALLEGRO_SAMPLE_INSTANCE **instances = audio_sample_instances->elements;
    ALLEGRO_SAMPLE_INSTANCE *current_instance = instances[audio_sample_instances->num_elements - 1];

    if (current_instance != NULL) {

        al_set_sample_instance_gain(current_instance, sample.sample_gain);
        al_set_sample_instance_pan(current_instance, sample.sample_pan);
        al_attach_sample_instance_to_mixer(current_instance, al_get_default_mixer());

    }

}

void audio_init_samples() 
{

    audio_sample_instances = Vector_Create(sizeof(ALLEGRO_SAMPLE_INSTANCE *), 8);

    audio_samples = dbaudio_get_samples();
    AudioSample *audio_samples_temp = audio_samples->elements;
    for (size_t i = 0; i < audio_samples->num_elements;i++)
        audio_load_sample(audio_samples_temp[i]);

}
    
void audio_init()
{

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(MAX_SAMPLES);

    dbaudio_init();
    audio_init_samples();

}

void audio_start_sample(ALLEGRO_SAMPLE_INSTANCE *instance) 
{

    if (instance != NULL) {

        al_set_sample_instance_position(instance, 0);
        al_play_sample_instance(instance);
        
    }

}

void audio_play_sample(uint32_t sample_id) 
{

    assert(sample_id != 0);

    AudioSample *audio_samples_temp = audio_samples->elements;
    ALLEGRO_SAMPLE_INSTANCE **instances = audio_sample_instances->elements;

    for (size_t i = 0; i < audio_samples->num_elements;i++) {

        if (audio_samples_temp[i].sample_id == sample_id) {

            audio_start_sample(instances[i]);
            break;

        }

    }


}

uint32_t audio_get_sample_id(char *sample_name)
{

    AudioSample *audio_samples_temp = audio_samples->elements;
    for (size_t i = 0; i < audio_samples->num_elements;i++)
        if (strcmp(audio_samples_temp[i].sample_name, sample_name) == 0)
            return audio_samples_temp[i].sample_id;

    return 0;

}

void audio_play_stream(AudioStream stream)
{



}