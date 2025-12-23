#pragma once
#include "System.hpp"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void HitWall(Event& event);

private:
    void AccumulateForces();

    float jump_height;

    float time_walking = 0.f;

    const float time_to_accel = 2.5f;
    const float time_to_decel = 0.25f;
    const float max_speed = 500.f;
    const float min_speed = 100.f;

    Vector2 forces;
    Vector2 v;

    float jump_impulse = 10.0f;

    float gravity = 980.f;
};