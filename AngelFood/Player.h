#pragma once
#include "Game.h"

enum PLAYER_STATES
{
    IDLE = 1,
    FALL = 2,
    WALK = 100,
    JUMP = 98,
    GLIDE = 99,
    DEAD = 103,
    HURT = 102,
};

struct player
{
    bool on_ground;
    HOLDABLE_ITEMS holding;
};