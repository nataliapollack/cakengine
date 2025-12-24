#pragma once

enum PLAYER_STATES
{
    IDLE,
    WALK,
    JUMP,
    FALL, 
    FELL
};

enum HOLDABLE_ITEMS
{
    NONE,
    TEMP
};

struct player
{
    bool on_ground;
    HOLDABLE_ITEMS holding;

    float vx;
    float vy;
};