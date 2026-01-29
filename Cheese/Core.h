#pragma once
#include "Game.h"
#include "raylib.h"

struct status
{
    // does this need to keep updating
    bool active;

    // does this need to keep drawing
    bool drawn;

    // do we need to reorganize asset order?
    bool dirty;

    // what is this
    OBJ_TYPES type;
};

struct transform25D
{
    Vector2 pos;
};

struct collision
{
    Rectangle box;
};

struct render25D
{
    ASSETS txt;
    float size;
};

struct render
{
    Vector2 dim;
    Color col;
};

struct animate
{
    int columns;
    int rows;
    int total_frames;
    int current_frame;
    int current_col;
    int current_row;
};