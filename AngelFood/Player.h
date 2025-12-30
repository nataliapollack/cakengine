#pragma once
#include "Game.h"

enum PLAYER_STATES
{
    IDLE = 1,
    WALK = 3,
    JUMP = 4,
    FALL = 2
};

struct player
{
    bool on_ground;
    HOLDABLE_ITEMS holding;
};