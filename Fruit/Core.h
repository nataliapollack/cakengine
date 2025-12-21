#pragma once
#include "raylib.h"
#include "Game.h"

struct transform2
{
    Vector2 pos;
};

struct render_box
{
    Vector2 size;
    Color color;
    Texture2D txt;
};

struct collidible
{
    Vector2 size;
    OBJ_TYPES type;
};

struct status
{
    bool active;
    bool dead;
};