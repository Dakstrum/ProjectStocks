#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "dbaccount.h"
#include "text.h"
#include "scrollbox.h"
#include "account.h"
#include "simulation.h"
#include "generalpurposemenus.h"

DrawObject *GetJSONObjectAndAddToDrawLayer(char* object_name)
{

    DrawObject *object = GetDrawObjectFromJsonLayer(object_name);
    AddObjectToDrawLayer(object);
    return object;

}