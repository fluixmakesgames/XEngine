#include "xe.c"

#define MAX_OBJECTSEDITOR 20

flat_gameobject objects[MAX_OBJECTSEDITOR];
int object_count = 0;

void New_Obj(char *name) {
    if(object_count < MAX_OBJECTSEDITOR) {
        objects[object_count].id = name;
        object_count++;
    }
}

bool input = false;

void Update() {
    BoxOnObject("ListObjBG",0,0,150,800,GRAY);
    BoxOnObject("ListObjBG",0,0,800,50,GRAY);
    BoxOnObject("ListObjBG",0,50,800,5,DARKGRAY);
    BoxOnObject("ListObjBG",150,0,5,800,DARKGRAY);
    
    BoxOnObject("NewObj",0,0,50,50,GRAY);
    TextOnObject("NewObj","+",10,0,50, WHITE);
    
    for(int i = 0; i < object_count; i++) {
        TextOnObject("ListObj", objects[i].id,10, 75 + (i * 20), 20, WHITE);
    }
    
    if(MouseBtn1ClickedOnObject("NewObj")) {
        char *names[] = {
            "Object",
            "AnotherObject",
            "YetAnotherObject",
            "ORB",
            "CHICK",
            "COW",
            "DUCK",
            "8",
            "9",
            "10",
            "11",
            "TWEVEL",
            "THREETEN",
            "FORTEIN",
            "FIIVE",
            "16",
            "17",
            "ORBTEN",
            "19",
            "20"
        };
        New_Obj(names[object_count]);
    }

}

void Start() {
    CreateFlatObject((Vector2){0,0},(Vector2){0,0}, "empty", "ListObjBG");
    CreateFlatObject((Vector2){0,0},(Vector2){50,50}, "empty", "NewObj");
    CreateFlatObject((Vector2){0,0},(Vector2){0,0}, "empty", "ListObj");
}

int main() {
    init(800, 450, "XEditor");
}