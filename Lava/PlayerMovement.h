#pragma once
#include "System.hpp"
#include "Events.hpp"

class PlayerMovementSystem : public System
{
public:
    void init();
    void move_player(float dt);
    void HitWall(Event& event);
private:
    float collision_forgiveness = 0.4f;
    float x_correction = 0.0f;
};