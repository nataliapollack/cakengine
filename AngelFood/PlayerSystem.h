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

    float max_glide_fall;

    size_t jump_charges;
    size_t jump_counter;

    bool should_jump;
    bool is_gliding;
};