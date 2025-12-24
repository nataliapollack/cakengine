#pragma once
#include "System.hpp"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void HitWall(Event& event);
    void PickedUpItem(Event& event);

private:
    void AccumulateForces();

    float time_walking = 0.f;

    float time_to_accel;
    float time_to_decel;
    float max_speed;
    float min_speed;

    float jump_impulse;

    float gravity;
};