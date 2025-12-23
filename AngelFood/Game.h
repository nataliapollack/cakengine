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