#pragma once
#include "System.hpp"
#include "Timer.hpp"

#include <chrono>

class TimeManager
{
public:
    TimeManager();
    void increment();
    bool needsFixedUpdate();
    void resetLastTime();
    const float getFixedDt() const;
    const float getDeltaTime() const;
    const float getTotalTime() const;
private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::chrono::steady_clock::time_point m_currTime;
    std::chrono::steady_clock::duration m_timeTaken;

    float m_accumulator;
    float m_time;
    float m_deltaTime;
    const float m_fixedDt = 0.02f;
};

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void PickedUpItem(Event& event);
    void DroppedItem(Event& event);

    void ResetPlayerPos();

    void SetGlideUnlocked(bool value);
    void SetDoubleJumpUnlocked(bool value);

private:
    void fixedUpdate();

    void WalkInput();
    void GlideInput(Entity entity);
    void JumpInput(Entity entity);

    void HitWall(Event& event);
    void AccumulateForces();

    struct walk_values
    {
        float time_to_accel;
        float time_to_decel;
        float max_speed;
        float min_speed;
        float time_walking;

        float direction;
    };
    walk_values m_walk;

    struct jump_values
    {
        // qol stuff
        Timer coyote_time;
        Timer jump_buffering;

        // jump height
        std::array<float, 2> jump_impulse;
        std::array<float, 2> jump_height;
        float jump_time;
        float jump_timer;

        // double jump
        size_t jump_charges;
        size_t jump_counter;

        bool should_jump;
        bool is_jumping;
    };
    jump_values m_jump;

    float gravity;

    // collision qol
    float collision_forgiveness;
    float x_correction;

    struct glide_values
    {
        Timer glide_time;
        float max_glide_fall;
        bool is_gliding;
        bool can_glide;
        bool glide_unlocked;
    };
    glide_values m_glide;

    Vector2 spawn_pos;

    std::unique_ptr<TimeManager> m_time;
};