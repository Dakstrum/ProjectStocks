
typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP

} DrawType;


typedef struct DrawObject 
{

    DrawType type;
    float x_pos;
    float y_pos;
    union {

        struct {

            char picture_path[256];

        } menu;

    } object;

} DrawObject;

typedef struct DrawLayer 
{



} DrawLayer;

static DrawLayer draw_layers[10];
static unsigned int current_draw_layer = 0;

void HandleMouseClickInButtonAreas(int x, int y) 
{

    
}