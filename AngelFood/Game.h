#pragma once
#include "Events.hpp"

const enum OBJECT_TYPE
{
    PLAYER,
    WALL,
    ITEM,
    DROPOFF
};

enum ASSETS
{
    PLAYER_IDLE,
    PLAYER_FALL,
    TEMP_ITEM,
    COUNT,
    EMPTY
};

namespace Events::Item {
    const EventId PICKEDUP = "Events::Item::PICKEDUP"_hash;
    const EventId DROPPEDOFF = "Events::Item::DROPPEDOFF"_hash;
}

namespace Events::Item::PickedUp
{
    const ParamId ITEMID = "Events::Item::PickedUp::ITEM_ID"_hash;
}

namespace Events::Collision {
    const EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const ParamId COLLISION_DATA = "Events::Item::IMPULSE_FORCE"_hash;
}