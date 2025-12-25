#pragma once
#include "Game.h"

enum PLAYER_STATES
{
    IDLE,
    WALK,
    JUMP,
    FALL, 
    FELL
};

struct player
{
    bool on_ground;
    HOLDABLE_ITEMS holding;
};