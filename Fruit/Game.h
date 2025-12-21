#pragma once
#include "raylib.h"
#include "Events.hpp"

enum OBJ_TYPES
{
    PLAYER,
    OBSTACLE,
    FLAG,
    FINISH_LINE
};

namespace Events::Collision {
    const ParamId OBJ_TYPE = "Events::Collision::OBJ_TYPE"_hash;
}

namespace Events::Obstacles {
    const EventId FLAGS = "Events::Obstacles::FLAGS"_hash;
    const EventId OBST = "Events::Obstacles::OBST"_hash;
    const EventId FINISH = "Events::Obstalces::FINISH"_hash;
}

namespace Events::Race {
    const EventId FINISHED = "Events::Race::FINISHED"_hash;
    const EventId STARTED = "Events::Race::STARTED"_hash;
}