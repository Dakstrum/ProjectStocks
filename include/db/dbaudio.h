#ifndef DBAUDIO_H
#define DBAUDIO_H

#include <stdint.h>
#include "vector.h"

typedef struct AudioSample {

	uint32_t sample_id;
    char sample_name[32];
    char sample_path[64];
    float sample_gain;
    float sample_pan;
    float sample_speed;

} AudioSample;

Vector *dbaudio_get_samples();
void dbaudio_init();

#endif