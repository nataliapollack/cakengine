#pragma once
#include "System.hpp"
#include "raylib.h"
#include "Events.hpp" 

class PlayerMovement : public System
{
public:
    void init();
    void update(float dt);

    void TriggerObstacleHit(Event& event);

private:
    bool can_jump;
    float current_jump_spd;
};