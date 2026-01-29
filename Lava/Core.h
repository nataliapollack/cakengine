#pragma once
#include "raylib.h"
#include "Lava.h"

struct transform2D
{
    Vector2 pos;
};

struct render_box
{
    Vector2 size;
    Color col;
};

struct collidble
{
    Rectangle box;
};

struct status
{
    // this is mainly for render / updating, we dont need to reorganzie the objs every frame unless one of them has changed
    // if a change gets applied to an entity, mark it as dirty
    // since this game is 2.5D, this change relies on the y axis
    // we make it as clean in render system 
    bool dirty;

    OBJECT_TYPE type;
};
