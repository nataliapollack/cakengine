#pragma once
#include "System.hpp"
#include "Timer.hpp"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void PickedUpItem(Event& event);
    void DroppedItem(Event& event);

    void ResetPlayerPos();

private:
    void ProcessJump(Entity entity, float dt);
    void HitWall(Event& event);
    void AccumulateForces();

    Timer coyote_time;
    Timer jump_buffering;

    float time_walking;

    float time_to_accel;
    float time_to_decel;
    float max_speed;
    float min_speed;

    std::array<float, 2> jump_impulse;
    std::array<float, 2> jump_height;
    float jump_time;

    float jump_timer;

    float gravity;

    float max_glide_fall;

    float collision_forgiveness = 0.4f;

    float x_correction = 0.0f;

    size_t jump_charges;
    size_t jump_counter;

    bool should_jump;
    bool is_jumping;
    bool is_gliding;

    Vector2 spawn_pos;
};