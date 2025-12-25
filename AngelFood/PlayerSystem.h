#pragma once
#include "System.hpp"
#include "Timer.hpp"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void HitWall(Event& event);
    void PickedUpItem(Event& event);

private:
    void AccumulateForces();

    Timer coyote_time;
    Timer jump_buffering;

    float time_walking;

    float time_to_accel;
    float time_to_decel;
    float max_speed;
    float min_speed;

    float jump_impulse;

    float gravity;

    bool should_jump;
};