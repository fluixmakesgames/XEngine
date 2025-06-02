#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROPS 200
#define MAX_OBJECTS 500
#define MAX_TAGS 300

typedef struct flat_gameobject flat_gameobject;

typedef struct {
    char *name;
    void *data;
    char *chardata;
    int intdata;
    bool booldata;
    flat_gameobject *parent;
    bool autorun;
} props;

struct flat_gameobject {
    Vector2 Position;
    Vector2 Transform;
    Texture2D Sprite;
    bool SpriteEmpty;
    char *id;
    props properties[MAX_PROPS];
    int p_count;
    char *tags[MAX_TAGS];
    int t_count;
};

typedef struct {
    Vector3 Position;
    Vector3 Transform;
    char *id;
} gameobject;

flat_gameobject flatobjs[MAX_OBJECTS];
int flatobjs_count = 0;

char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

flat_gameobject* GetFlatGameObject(char *id) {
    for (int i = 0; i < flatobjs_count; i++) {
        if (strcmp(id, flatobjs[i].id) == 0) {
            return &flatobjs[i];
        }
    }
    return NULL;
}

void CreateFlatObject(Vector2 Pos, Vector2 Trans, char *Spr, char *id) {
    if (flatobjs_count < MAX_OBJECTS) {
        flat_gameobject *obj = &flatobjs[flatobjs_count];

        obj->Position = Pos;
        obj->Transform = Trans;

        if (Spr == NULL || strcmp(Spr, "empty") == 0) {
            obj->SpriteEmpty = true;
        } else {
            obj->Sprite = LoadTexture(Spr);
            obj->SpriteEmpty = false;
        }

        obj->id = strdup_safe(id);
        obj->p_count = 0;

        flatobjs_count++;


    }
}


void New_Property(char *name, void *data, char *obj_name, bool runalways) {
    flat_gameobject *obj = GetFlatGameObject(obj_name);
    if (obj && obj->p_count < MAX_PROPS) {
        int idx = obj->p_count;

        obj->properties[idx].name = strdup_safe(name);
        obj->properties[idx].data = data;
        obj->properties[idx].parent = obj;
        obj->properties[idx].autorun = runalways;

        obj->p_count++;
    }
}

void New_CharProperty(char *name, char *data, char *obj_name) {
    flat_gameobject *obj = GetFlatGameObject(obj_name);
    if (obj && obj->p_count < MAX_PROPS) {
        int idx = obj->p_count;

        obj->properties[idx].name = strdup_safe(name);
        obj->properties[idx].chardata = data;
        obj->properties[idx].parent = obj;

        obj->p_count++;
    }
}

void New_IntProperty(char *name, int data, char *obj_name) {
    flat_gameobject *obj = GetFlatGameObject(obj_name);
    if (obj && obj->p_count < MAX_PROPS) {
        int idx = obj->p_count;

        obj->properties[idx].name = strdup_safe(name);
        obj->properties[idx].intdata = data;
        obj->properties[idx].parent = obj;

        obj->p_count++;
    }
}

void New_BoolProperty(char *name, bool data, char *obj_name) {
    flat_gameobject *obj = GetFlatGameObject(obj_name);
    if (obj && obj->p_count < MAX_PROPS) {
        int idx = obj->p_count;

        obj->properties[idx].name = strdup_safe(name);
        obj->properties[idx].booldata = data;
        obj->properties[idx].parent = obj;

        obj->p_count++;
    }
}

void New_Tag(char *tag, char *obj_name) {
    flat_gameobject *obj = GetFlatGameObject(obj_name);
    if (obj && obj->t_count < MAX_TAGS) {
        int idx = obj->t_count;

        obj->tags[idx] = strdup_safe(tag);

        obj->t_count++;
    }
}

bool HasTag(flat_gameobject *object, const char *tag) {
    if (!object) return false;

    for (int i = 0; i < object->t_count; i++) {
        if (strcmp(tag, object->tags[i]) == 0) {
            return true;
        }
    }
    return false;
}

void DrawObjects() {
    for (int i = 0; i < flatobjs_count; i++) {
        if(!flatobjs[i].SpriteEmpty)
        DrawTexture(flatobjs[i].Sprite, flatobjs[i].Position.x, flatobjs[i].Position.y, WHITE);
    }
}

props* GetProperty(flat_gameobject *object, const char *name) {
    if (!object) return NULL;

    for (int i = 0; i < object->p_count; i++) {
        if (strcmp(name, object->properties[i].name) == 0) {
            return &object->properties[i];
        }
    }
    return NULL;
}


void Run_Property(char *object, char *prop) {
    flat_gameobject *obj = GetFlatGameObject(object);
    if (!obj) return;

    props *property = GetProperty(obj, prop);
    if (!property || !property->data) return;

    void (*func)(props *) = (void (*)(props *))property->data;
    func(property);
}

bool MouseBtn1ClickedOnObject(char *obj) {
    flat_gameobject *object = GetFlatGameObject(obj);
    if (!object) return false;

    Rectangle area = {
        object->Position.x,
        object->Position.y,
        object->Transform.x,
        object->Transform.y
    };

    return CheckCollisionPointRec(GetMousePosition(), area) &&
           IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool MouseBtn2ClickedOnObject(char *obj) {
    flat_gameobject *object = GetFlatGameObject(obj);
    if (!object) return false;

    Rectangle area = {
        object->Position.x,
        object->Position.y,
        object->Transform.x,
        object->Transform.y
    };

    return CheckCollisionPointRec(GetMousePosition(), area) &&
           IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
}

bool MouseBtn1DownOnObject(char *obj) {
    flat_gameobject *object = GetFlatGameObject(obj);
    if (!object) return false;

    Rectangle area = {
        object->Position.x,
        object->Position.y,
        object->Transform.x,
        object->Transform.y
    };

    return CheckCollisionPointRec(GetMousePosition(), area) &&
           IsMouseButtonDown(MOUSE_LEFT_BUTTON);
}

bool MouseBtn2DownOnObject(char *obj) {
    flat_gameobject *object = GetFlatGameObject(obj);
    if (!object) return false;

    Rectangle area = {
        object->Position.x,
        object->Position.y,
        object->Transform.x,
        object->Transform.y
    };

    return CheckCollisionPointRec(GetMousePosition(), area) &&
           IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
}



bool ObjectsTouching(char *obj, char *obj2) {
    flat_gameobject *object = GetFlatGameObject(obj);
    if (!object) return false;
    
    flat_gameobject *object2 = GetFlatGameObject(obj2);
    if (!object2) return false;

    Rectangle area = {
        object->Position.x,
        object->Position.y,
        object->Transform.x,
        object->Transform.y
    };
    
    Rectangle area2 = {
        object2->Position.x,
        object2->Position.y,
        object2->Transform.x,
        object2->Transform.y
    };

    return CheckCollisionRecs(area, area2);
}

bool ObjectsTouchingWithTag(char *obj, char *tag) {
    
    flat_gameobject *object2 = NULL;
    
    for(int i = 0; i < flatobjs_count; i++) {
        if(HasTag(GetFlatGameObject(flatobjs[i].id), tag)) {
            object2 = GetFlatGameObject(flatobjs[i].id);
            if(ObjectsTouching(obj, object2->id)) return true;
        }
    }
    return false;
}

int GetCharDown() {
    for (int key = 0; key < 512; key++) { 
        if (IsKeyDown(key)) {
            return key; 
        }
    }
    return 0; 
}


char KeyDown() {
    int key = GetCharDown();

    return (char) key;
}

char KeyPressed() {
    int key = GetCharPressed();

    return (char) key;
}

void TextOnObject(char *object, char *text, int off_x, int off_y, int size, Color color) {
    flat_gameobject *obj = GetFlatGameObject(object);
    if (!obj) return;

    DrawText(text, obj->Position.x + off_x, obj->Position.y + off_y, size, color);
}

void BoxOnObject(char *object, int off_x, int off_y, int width, int height, Color color) {
    flat_gameobject *obj = GetFlatGameObject(object);
    if (!obj) return;

    DrawRectangle(obj->Position.x + off_x, obj->Position.y + off_y, obj->Transform.x + width, obj->Transform.y + height, color);
}

void runproperties() {
    for(int i = 0; i < flatobjs_count; i++) {
        flat_gameobject *obj = GetFlatGameObject(flatobjs[i].id);
        if(!obj) return;
        
        for(int j = 0; j < obj->p_count; j++) {
            props *prop = &obj->properties[j];
            if (prop->autorun && prop->data) {
                void (*func)(props *) = (void (*)(props *))prop->data;
                if(prop->autorun) func(prop);
            }
        }
    }
}

void TopDownPlayerMovement(props *self) {
    flat_gameobject *parent = self->parent;
    
    if(IsKeyDown(KEY_RIGHT)) parent->Position.x += 5;
    if(IsKeyDown(KEY_LEFT)) parent->Position.x -= 5;
    
    if(IsKeyDown(KEY_DOWN)) parent->Position.y += 5;
    if(IsKeyDown(KEY_UP)) parent->Position.y -= 5;
}

void Update();
void Start();

void init(int w, int h, char *t) {
    InitWindow(w, h, t);

    Start();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawObjects();
        Update();
        runproperties();

        EndDrawing();
    }

    CloseWindow();
}

