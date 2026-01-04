#pragma once
#include "Game.h"

struct collectable
{
    bool picked_up;
};

struct collecting
{
    HOLDABLE_ITEMS item;
    int amount_needed;
};

struct spike
{
};

struct spark
{
    int frame_counter;
    ASSETS current_asset;
};

struct NPC
{
};

struct Spark
{
    bool picked_up;

};

struct waiting_game
{
    float max_time;
};