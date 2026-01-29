#pragma once
#include "System.hpp"

class PlayerMovementSystem : public System
{
public:
    void move_player(float dt);
};