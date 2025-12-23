#pragma once


const enum OBJECT_TYPE
{
    PLAYER,
    WALL,
    ITEM,
    DROPOFF
};


namespace Events::Item {
    const EventId PICKED_UP = "Events::Item::PICKED_UP"_hash;
    const EventId DROPPED_OFF = "Events::Item::DROPPED_OFF"_hash;
    const ParamId OBJ_TYPE = "Events::Item::OBJ_TYPE"_hash;
}

namespace Events::Collision {
    const EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const ParamId IMPULSE_FORCE = "Events::Item::IMPULSE_FORCE"_hash;
}