// this is the Game.h file

#include "Events.hpp"

enum OBJECT_TYPE
{
    PLAYER,
    DOOR,
    RADIO_OBJ,
    ENERGY_SCREEN,
    SOLAR_SCREEN,
    ENVIRONMENT_OBJ,
    MAZE_SCREEN,
    WALL
};

namespace Events::Collision {
    const ParamId COLLISION_DATA = "Events::Collision::IMPULSE_FORCE"_hash;
    const EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const EventId HIT_DOOR = "Events::Collision::HIT_DOOR"_hash;
}

namespace Events::Energy {
    const EventId ENERGY_DOWN = "Events::Energy::ENERGY_DOWN"_hash;
    const EventId ENERGY_UP = "Events::Energy::ENERGY_UP"_hash;
    const ParamId ENERGY_TICK = "Events::Energy::ENERGY_TICK"_hash;
}

namespace Events::Health {
    const EventId HEALTH_DMG = "Events::Health::HEALTH_DMG"_hash;
}

namespace Events::Game {
    const EventId START = "Events::Game::START"_hash;
    const EventId END = "Events::Game::END"_hash;
    const EventId SCREEN_CHANGE = "Events::Game::SCREEN_CHANGE"_hash;
    const ParamId SCREEN_ID = "Events::Game::SCREEN_ID"_hash;
}