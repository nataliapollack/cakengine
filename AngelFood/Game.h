#pragma once
#include "Events.hpp"

const enum OBJECT_TYPE
{
    PLAYER,
    WALL,
    ITEM,
    DROPOFF,
    SPIKES,
    SPAWNER,
    ENVIRONMENT,
    PLATFORM,
    SPARK,
    EMITTER, 
    NPC
};

enum HOLDABLE_ITEMS
{
    NONE,
    TEMP = 3
};

enum ASSETS
{
    HALO,
    PLAYER_IDLE,
    PLAYER_FALL,
    DAEDALUS_IDLE,
    KING_IDLE1,
    KING_IDLE2,
    BIRD_IDLE1,
    BIRD_IDLE2,
    SHEEP_IDLE1,
    SHEEP_IDLE2,
    SNAKE_IDLE1,
    SNAKE_IDLE2,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4,
    ROOM5,
    ROOM6,
    ROOM7,
    ROOM8,
    ROOM9,
    ROOM10,
    ROOM11,
    ROOM12, 
    ROOM13,
    ROOM14,
    ROOM15,
    BRANCH1,
    BRANCH2,
    FLOATING1,
    FLOATING2,
    EDGE1,
    EDGE2,
    EDGE3,
    GROUND1,
    GROUND2,
    DARK,
    SIDE1,
    SIDE2,
    SIDE3,
    GLASS1,
    GLASS2,
    GLASS3,
    BUSH1,
    BUSH2,
    BUSH3,
    ENV_BRANCH1,
    ENV_BRANCH2,
    ENV_BRANCH3,
    ENV_BRANCH4,
    GRASS1,
    GRASS2,
    GRASS3,
    ROOTS,
    BG_BRANCH,
    BG_BUSH1,
    BG_BUSH2,
    BG_VINE1,
    BG_VINE2,
    BG_ROOT1,
    BG_ROOT2,
    BG_ROOT3,
    BG_BUSH3,
    BG_BUSH4,
    BG_BUSH5,
    BG_BUSH6,
    BG_BUSH7,
    BG_BUSH8,
    HANGING1,
    HANGING2,
    HANGING3,
    HANGING4,
    VINE1,
    VINE2,
    VINE3,
    VINE4,
    FRUIT1,
    FRUIT2,
    KING_HAPPY1,
    KING_HAPPY2,
    BIRD_HAPPY1,
    BIRD_HAPPY2,
    SHEEP_HAPPY1,
    SHEEP_HAPPY2,
    SNAKE_PLAY1,
    SNAKE_PLAY2,
    SPARK1,
    SPARK2,
    SPARK3,
    SPARK4,
    COUNT
};

namespace Events::Item {
    const EventId PICKEDUP = "Events::Item::PICKEDUP"_hash;
    const EventId DROPPEDOFF = "Events::Item::DROPPEDOFF"_hash;
}

namespace Events::Item::PickedUp
{
    const ParamId ITEMID = "Events::Item::PickedUp::ITEMID"_hash;
    const ParamId OBJTYPE = "Events::Item::PickedUp::OBJTYPE"_hash;
}

namespace Events::Item::DroppedOff
{
    const ParamId ITEMID = "Events::Item::DroppedOff::ITEMID"_hash;
}

namespace Events::Collision {
    const EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const ParamId COLLISION_DATA = "Events::Item::IMPULSE_FORCE"_hash;
    const EventId SPIKES = "Events::Collision::SPIKES"_hash;
    const EventId SPAWNER = "Events::Collision::SPAWNER"_hash;
    const EventId SPARK = "Events::Collision::SPARK"_hash;
}