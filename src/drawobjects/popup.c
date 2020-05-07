#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro.h>

#include "draw.h"
#include "cache.h"
#include "shared.h"
#include "drawobject.h"

void PopupSetUpDy(void *gen_object, int x, int y)
{
    
    DrawObject *object = (DrawObject *)gen_object;
    object->popup.dy   = -(float)(object->popup.current_y - y)/(object->popup.diff_time_left_to_animate);

}

void PopupSetDownDy(void *gen_object, int x,  int y)
{

    DrawObject *object = (DrawObject *)gen_object;
    object->popup.dy   = (float)(y - object->popup.current_y)/(object->popup.diff_time_left_to_animate);

}

void PopupSetLeftDx(void *gen_object, int x, int y)
{

    DrawObject *object = (DrawObject *)gen_object;
    object->popup.dx   = -(float)(object->popup.current_x - x)/(object->popup.diff_time_left_to_animate);

}

void PopupSetRightDx(void *gen_object, int x, int y)
{

    DrawObject *object = (DrawObject *)gen_object;
    object->popup.dx   = (float)(x - object->popup.current_x)/(object->popup.diff_time_left_to_animate);
    
}

void PopupSetInitialPositionAndAnimationFunction(DrawObject *object,  const int display_width, const int display_height)
{

    object->popup.dx         = 0;
    object->popup.dy         = 0;
    object->popup.current_x  = (float)object->x;
    object->popup.current_y  = (float)object->y;

    if (strcmp(object->popup.direction, "Up") == 0) {

        object->popup.current_y         = display_height;
        object->popup.set_dx_dy         = &PopupSetUpDy;
        object->popup.set_dx_dy_reverse = &PopupSetDownDy;

    } else if (strcmp(object->popup.direction, "Down") == 0) {

        object->popup.current_y         = -object->height;
        object->popup.set_dx_dy         = &PopupSetDownDy;
        object->popup.set_dx_dy_reverse = &PopupSetUpDy;

    } else if (strcmp(object->popup.direction, "Left") == 0) {

        object->popup.current_x         = display_width;
        object->popup.set_dx_dy         = &PopupSetLeftDx;
        object->popup.set_dx_dy_reverse = &PopupSetRightDx;

    } else if (strcmp(object->popup.direction, "Right") == 0) {

        object->popup.current_x         = -object->width;
        object->popup.set_dx_dy         = &PopupSetRightDx;
        object->popup.set_dx_dy_reverse = &PopupSetLeftDx;

    }

    object->popup.start_x = object->popup.current_x;
    object->popup.start_y = object->popup.current_y;

}

void InitPopup(DrawObject *object, const int display_width, const int display_height)
{

    if (object->asset_path != NULL)
        object->popup.popup_bitmap = GetBitmapFromCache(object->asset_path);

    object->popup.last_animation_time       = GetCurrentTime();
    object->popup.diff_time_left_to_animate = object->popup.diff_time_to_animate;
    object->popup.diff_time_left_to_stay    = object->popup.diff_time_to_stay;

    PopupSetInitialPositionAndAnimationFunction(object, display_width, display_height);

}

void MovePopUp(DrawObject *object, void (*set_dx_dy)(void *, int, int), bool reversed, int bit_flag)
{

    struct timespec current_time = GetCurrentTime();
    long dt_milli                = GetMillDiff(&current_time, &object->popup.last_animation_time);

    if (dt_milli == 0)
        return;

    object->popup.diff_time_left_to_animate -= dt_milli;
    if (object->popup.diff_time_left_to_animate > 0) {

        object->popup.last_animation_time = current_time;
        if (!reversed)
            set_dx_dy(object, object->x, object->y);
        else
            set_dx_dy(object, object->popup.start_x, object->popup.start_y);

        object->popup.current_x += object->popup.dx * dt_milli;
        object->popup.current_y += object->popup.dy * dt_milli;

    } else {

        object->bit_flags |= bit_flag;
        object->popup.diff_time_left_to_animate = object->popup.diff_time_to_animate;

    }

}

void WaitPopUp(DrawObject *object)
{

    struct timespec current_time = GetCurrentTime();
    if (GetMillDiff(&current_time, &object->popup.last_stay_time) >= object->popup.diff_time_to_stay)
        object->bit_flags |= POPUP_DONE_STAYING;

}

void DrawPopUp(DrawObject *object)
{

    if (!(object->bit_flags & POPUP_DONE_INTRO_ANIMATION)) {

        MovePopUp(object, object->popup.set_dx_dy, false, POPUP_DONE_INTRO_ANIMATION);
        if (object->bit_flags & POPUP_DONE_INTRO_ANIMATION)
            object->popup.last_stay_time = GetCurrentTime();

    } else if (!(object->bit_flags & POPUP_DONE_STAYING)) {

        WaitPopUp(object);
        if (object->bit_flags & POPUP_DONE_STAYING)
            object->popup.last_animation_time = GetCurrentTime();

    } else if (!(object->bit_flags & POPUP_DONE_OUTRO_ANIMATION)) {

        MovePopUp(object, object->popup.set_dx_dy_reverse, true, POPUP_DONE_OUTRO_ANIMATION);

    } else {

        object->bit_flags |= CAN_BE_REMOVED;
        return;

    }

    DrawGenericWithWidth(object->popup.popup_bitmap, (int)object->popup.current_x, (int)object->popup.current_y, object->width, object->height);
    al_draw_text(object->popup.text_style->font, object->popup.text_style->color, (int)object->popup.current_x+650, (int)object->popup.current_y+(1000), 0, object->popup.text);

}

void SetPopupText(DrawObject *object, const char *text)
{

    strncpy(object->popup.text, text, 128);
    object->popup.text[127] = '\0';

}

DrawObject *CreateNewPopup()
{

    DrawObject *object = CreateNewDrawObject();

    object->type   = POPUP;
    object->x      = 0;
    object->y      = 0;
    object->width  = 1920;
    object->height = 1080;
    
    object->asset_path                 = "assets/images/generalpurposemenus/popups/genericpopup.png";
    object->popup.diff_time_to_animate = 1000;
    object->popup.diff_time_to_stay    = 2000;
    object->popup.direction            = "Up";
    object->popup.text_style           = malloc(sizeof(TextStyle));
    memset(object->popup.text, '\0', 128);

    object->popup.text_style->font  = GetFontFromCache("assets/font/DanielLinssenM5/m5x7.ttf", 48);
    object->popup.text_style->color = al_map_rgba(0, 0, 0, 255);

    return object;

}

void CleanUpPopUp(DrawObject *object) 
{
    
    if (object == NULL)
        return;

    free(object->popup.text_style);

}