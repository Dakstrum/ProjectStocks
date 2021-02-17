
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dbutils.h"
#include "dbaudio.h"


Vector *db_audio_samples = NULL;

Vector *dbaudio_get_samples()
{

	assert(db_audio_samples != NULL);
	return db_audio_samples;

}

int dbaudio_audio_sample_callback(void *nil, int argc, char **argv, char **col_name)
{

	if (argc == 0)
		return 0;

	AudioSample sample;
	sample.sample_id = atoi(argv[0]);
	strncpy(sample.sample_name, argv[1], 32);
	strncpy(sample.sample_path, argv[2], 64);

	sample.sample_name[31] = '\0';
	sample.sample_path[63] = '\0';

	sample.sample_gain  = atof(argv[3]);
	sample.sample_pan   = atof(argv[4]);
	sample.sample_speed = atof(argv[5]);

	Vector_PushBack(db_audio_samples, &sample);

    return 0;

}


void dbaudio_init()
{

	db_audio_samples = Vector_Create(sizeof(AudioSample), 8);

	char *audio_sample_query = "SELECT * FROM System_Audio_Samples";
	ExecuteQueryF(dbaudio_audio_sample_callback, NULL, audio_sample_query);


}