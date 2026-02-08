// this is the Game.h file
#pragma once

#include "Events.hpp"

enum OBJECT_TYPE
{
    PLAYER,
    DOOR,
    RADIO_OBJ,
    SOLAR_OBJ,
    MAZE_OBJ,
    CAM_OBJ,
    ENVIRONMENT_OBJ,
    WALL
};

namespace Events::Collision {
    const inline ParamId COLLISION_DATA = "Events::Collision::IMPULSE_FORCE"_hash;
    const inline EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const inline EventId HIT_DOOR = "Events::Collision::HIT_DOOR"_hash;
    const inline EventId HIT_SOLAR = "Events::Collision::HIT_SOLAR"_hash;
    const inline EventId HIT_MAZE = "Events::Collision::HIT_MAZE"_hash;
    const inline EventId HIT_CAMS = "Events::Collision::HIT_CAMS"_hash;
    const inline EventId HIT_AUDIO = "Events::Collision::HIT_AUDIO"_hash;
}

namespace Events::Energy {
    const inline EventId ENERGY_DOWN = "Events::Energy::ENERGY_DOWN"_hash;
    const inline EventId ENERGY_UP = "Events::Energy::ENERGY_UP"_hash;
    const inline ParamId ENERGY_TICK = "Events::Energy::ENERGY_TICK"_hash;
}

namespace Events::Health {
    const inline EventId HEALTH_DMG = "Events::Health::HEALTH_DMG"_hash;
}

namespace Events::Game {
    const inline EventId START = "Events::Game::START"_hash;
    const inline EventId END = "Events::Game::END"_hash;
    const inline EventId SCREEN_CHANGE = "Events::Game::SCREEN_CHANGE"_hash;
    const inline ParamId SCREEN_ID = "Events::Game::SCREEN_ID"_hash;
}

namespace Events::Time {
    const inline EventId NIGHT_BEGIN = "Events::Time::NIGHT_BEGIN"_hash;
    const inline EventId DAY_BEGIN = "Events::Time::DAY_BEGIN"_hash;
}

namespace Events::Hints {
    const inline EventId HINT_RECEIVED = "Events::Hints::HINT_RECEIVED"_hash;
}

namespace Events::Dialogue {
    const inline EventId TUTORIAL = "Events::Dialogue::TUTORIAL"_hash;
    const inline EventId AUDIO_FIXED = "Events::Dialogue::AUDIO_FIXED"_hash;
    const inline ParamId ID = "Events::Dialogue::TUTORIAL_ID"_hash;
}