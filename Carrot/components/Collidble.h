#pragma once
#include "raylib.h"

enum OBJ_TYPES
{
    PLAYER,
    GIRL1,
    GIRL2,
    GIRL3,
    FAKE,
    DOOR,
    LOCKEDDOOR,
    EXITDOOR
};

struct Collidible
{
    OBJ_TYPES type;
    Vector2 collision_box;
};