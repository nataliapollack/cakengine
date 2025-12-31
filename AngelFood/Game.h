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
    PLATFORM
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
    ROOM5,
    ROOM6,
    ROOM7,
    ROOM8,
    ROOM9,
    ROOM10,
    ROOM11,
    COUNT
};

enum TILE_ASSETS
{
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
    TILE_COUNT
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
}