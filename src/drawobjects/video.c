
#include <allegro5/allegro_video.h>

#include "log.h"
#include "draw.h"
#include "drawobject.h"

void InitVideo(DrawObject *object)
{

    object->video.video = al_open_video(object->asset_path);
    if (object->video.video == NULL) {

        LogF("Unable to open %s", object->asset_path);
        return;
    }
    
    if (object->bit_flags & VIDEO_SHOULD_START_IMMEDIATELY)
        al_start_video(object->video.video, al_get_default_mixer());

}

void DrawVideo(DrawObject *object) 
{

    if (!al_is_video_playing(object->video.video) && object->bit_flags & VIDEO_SHOULD_REPEAT) {

        al_seek_video(object->video.video, 0.0);
        al_set_video_playing(object->video.video, true);

    }

    DrawGenericWithWidth(al_get_video_frame(object->video.video), object->x, object->y, object->width, object->height);
    
}


void CleanUpVideo(DrawObject *object) 
{

    al_close_video(object->video.video);

}