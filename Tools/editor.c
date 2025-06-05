#include "xe.c"

void blankvoid() {
}

#define MAX_OBJECTSEDITOR 20

flat_gameobject *objects[MAX_OBJECTSEDITOR];
int object_count = 0;
bool editormode = true;
bool sel = false;
flat_gameobject *sel_obj;
int sel_x = 0;
int sel_y = 0;
bool guisel = false;

int PackColor(Color c) {
    return (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a;
}

Color UnpackColor(int packed) {
    return (Color){
        (packed >> 24) & 0xFF,
        (packed >> 16) & 0xFF,
        (packed >> 8) & 0xFF,
        packed & 0xFF
    };
}


void RenderBox(props *self) {
    flat_gameobject *parent = self->parent;
    props *color = GetProperty(parent, "Color");
    BoxOnObject(parent->id,0,0,0,0,UnpackColor(color->intdata));
}

void EditorEdit(props *self) {
    if(editormode) {
        flat_gameobject *parent = self->parent;
    
        if(MouseBtn1DownOnObject(parent->id) && !sel) {
            sel = true;
            sel_obj = parent;
        } else if(sel_obj) if(strcmp(sel_obj->id, parent->id) == 0) {
            if(!MouseBtn1DownOnObject(parent->id) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !guisel) {
                sel = false;
                sel_obj = NULL;
            }
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && sel && !guisel) {
                parent->Position.x = GetMouseX() - parent->Transform.x/2;
                parent->Position.y = GetMouseY() - parent->Transform.y/2;
                sel_x = parent->Position.x - 155;
                sel_y = parent->Position.y - 55;
            }
        }
    }
}

void New_Obj(char *name, Color color) {
    if(object_count < MAX_OBJECTSEDITOR) {
        CreateFlatObject((Vector2){155,55},(Vector2){60,60}, "empty", strdup(name));
        New_Property("EditorEdit", EditorEdit, strdup(name), true);
        New_Property("RenderBox", RenderBox, strdup(name), false);
        New_IntProperty("Color", PackColor(color), strdup(name));
        objects[object_count] = GetFlatGameObject(strdup(name));
        object_count++;
    }
}

bool input = false;

char buffer[12];
int bufferlength = 0;
char *prompt = "";

void Update() {
    
    for(int i = 0; i < object_count; i++) {
        Run_Property(objects[i]->id, "RenderBox");
    }
    
    BoxOnObject("ListObjBG",0,50,800,5,DARKGRAY);
    BoxOnObject("ListObjBG",150,0,5,800,DARKGRAY);
    BoxOnObject("ListObjBG",155,305,450,145,GRAY);
    BoxOnObject("ListObjBG",605,0,350,450,GRAY);
    //BoxOnObject("SceneArea",0,0,0,0,(Color){155,0,0,155});
    //TextOnObject("SceneArea","Scene Area",10,10,50, WHITE);
    BoxOnObject("ListObjBG",0,300,800,5,DARKGRAY);
    BoxOnObject("ListObjBG",155,305,450,145,GRAY);
    BoxOnObject("ListObjBG",605,0,350,450,GRAY);
    BoxOnObject("ListObjBG",605,0,5,800,DARKGRAY);
    BoxOnObject("ListObjBG",0,0,150,800,GRAY);
    BoxOnObject("ListObjBG",0,0,800,50,GRAY);

    BoxOnObject("NewObj",0,0,0,0,(Color){165,165,165,255});
    TextOnObject("NewObj","+O",10,10,15, WHITE);
    
    BoxOnObject("NewProp",0,0,0,0,(Color){165,165,165,255});
    TextOnObject("NewProp","+P",10,10,15, WHITE);
    
    TextOnObject("ListObj","Object:",610,10,20, WHITE);
    if(sel_obj) {
        TextOnObject("ListObj",TextFormat("%s", sel_obj->id),630,40,20, WHITE);
        TextOnObject("ListObj",TextFormat("X:%i", sel_x),640,60,20, WHITE);
        TextOnObject("ListObj",TextFormat("Y:%i", sel_y),720,60,20, WHITE);
    } else {
        TextOnObject("ListObj","No Object",630,40,20, WHITE);
        TextOnObject("ListObj","X:",640,60,20, WHITE);
        TextOnObject("ListObj","Y:",720,60,20, WHITE);
    }
    for(int i = 0; i < object_count; i++) {
        TextOnObject("ListObj", objects[i]->id,10, 75 + (i * 20), 20, WHITE);
    }
    
    TextOnObject("ListObj","Properties:",630,125,20, WHITE);
    
    if (sel_obj) {
        for (int i = 1; i < sel_obj->p_count; i++) {
            TextOnObject("ListObj", GetFlatGameObject(sel_obj->id)->properties[i].name, 630, 120 + (i * 35), 20, WHITE);
        }
    }

    
    if(input) {
        BoxOnObject("ListObj", 260,190,420,100, DARKGRAY);
        BoxOnObject("ListObj", 270,200,400,80, GRAY);
        TextOnObject("ListObj", TextFormat("%s",buffer),300, 225, 20, WHITE);
        if (IsKeyPressed(KEY_ENTER)) {
            if(bufferlength>0) {
                buffer[bufferlength] = '\0';
                if(strcmp(prompt,"obj") == 0) New_Obj(buffer, BLUE);
                if(strcmp(prompt,"prop") == 0) {
                    if(sel_obj) New_Property(strdup(buffer), blankvoid, sel_obj->id, false);
                }
                bufferlength = 0;
                memset(buffer, 0, sizeof(buffer));
                input = false;
            }
        } else if (IsKeyPressed(KEY_BACKSPACE)) {
            if(bufferlength>0) {
                bufferlength--;
                buffer[bufferlength] = ' ';
            }
        } else {
            char c = KeyPressed();
            if(c && bufferlength < (int)sizeof(buffer) - 1) {
                buffer[bufferlength++] = c;
            }
        }
    }
    
    if(MouseBtn1ClickedOnObject("NewObj")) {
        input = true;
        bufferlength = 0;
        prompt = "obj";
        memset(buffer, 0, sizeof(buffer));
    }
    if(MouseBtn1ClickedOnObject("NewProp")) {
        input = true;
        bufferlength = 0;
        prompt = "prop";
        memset(buffer, 0, sizeof(buffer));
    }
    if(MouseBtn1ClickedOnObject("SceneArea")) {
        guisel = false;
    } else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) guisel = true;
}

void Start() {
    CreateFlatObject((Vector2){0,0},(Vector2){50,50}, "empty", "NewObj");
    CreateFlatObject((Vector2){60,0},(Vector2){50,50}, "empty", "NewProp");
    CreateFlatObject((Vector2){0,0},(Vector2){0,0}, "empty", "ListObjBG");
    CreateFlatObject((Vector2){0,0},(Vector2){0,0}, "empty", "ListObj");
    CreateFlatObject((Vector2){155,55},(Vector2){450,250}, "empty", "SceneArea");
}

int main() {
    init(800, 450, "XEditor");
}