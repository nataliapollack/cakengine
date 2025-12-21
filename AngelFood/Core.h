#pragma once
#include "raylib.h"

struct transform2D
{
    Vector2 pos;
};

struct render
{
    Vector2 size;
    Color color;
};

struct collidble
{
    Rectangle box;
};

// this one is for like. if an entity is not rlly GONE, but we dont wanna draw/ update it rlly right now.
// so all systems should check if it's active first before doing schtuff
struct status
{
    bool active;
};