#pragma once
#include "raylib.h"
#include "Game.h"

struct transform2D
{
    Vector2 pos;
};

// we'll add txtures here and eventually animation component
struct render
{
    float size;
    ASSETS txt;
};

struct box_render
{
    Vector2 dimensions;
    Color color;
};

struct animate
{
    Vector2 size_dest;
};

struct collidble
{
    Rectangle box;
};

struct physics
{
    Vector2 vel;
    Vector2 f;
};

struct status
{
    bool active;
    // this one is for like. if an entity is not rlly GONE, but we dont wanna draw/ update it rlly right now.
    // so all systems should check if it's active first before doing schtuff
    bool dirty;
    // this is mainly for render / updating, we dont need to reorganzie the objs every frame unless one of them has changed
    // if a change gets applied to an entity, mark it as dirty

    OBJECT_TYPE type;
};
