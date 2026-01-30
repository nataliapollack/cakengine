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
    MAZE_SCREEN
};

namespace Events::Collision {
    const EventId HIT_WALL = "Events::Collision::COLLIDED"_hash;
    const ParamId COLLISION_DATA = "Events::Collision::IMPULSE_FORCE"_hash;
}

namespace Events::Energy {
    const EventId ENERGY_DOWN = "Events::Energy::ENERGY_DOWN"_hash;
    const EventId ENERGY_UP = "Events::Energy::ENERGY_UP"_hash;
    const ParamId ENERGY_TICK = "Events::Energy::ENERGY_TICK"_hash;
}

namespace Events::Game {
    const EventId START = "Events::Game::START"_hash;
    const EventId END = "Events::Game::END"_hash;
}