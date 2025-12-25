#pragma once
#include "Events.hpp"

const enum OBJECT_TYPE
{
    PLAYER,
    WALL,
    ITEM,
    DROPOFF
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
    TEMP_ITEM,
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
}