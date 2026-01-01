#pragma once
#include "System.hpp"
#include "Timer.hpp"
#include "Player.h"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

    void update_state();

    void PickedUpItem(Event& event);
    void DroppedItem(Event& event);

    void ResetPlayerPos();
    void HitSpikes(Event& event);
    void HitSpawner(Event& event);

    void SetGlideUnlocked(bool value);
    void SetDoubleJumpUnlocked(bool value);

private:
    PLAYER_STATES current_state;

    void fixedUpdate(float dt, Entity entity);

    void WalkInput();
    void GlideInput(Entity entity);
    void JumpInput(Entity entity, float dt);

    void HitWall(Event& event);
    void AccumulateForces();

    void HitSpark(Event& event);

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

        float move_multiplier;

        // jump height
        std::array<float, 2> jump_impulse;
        std::array<float, 2> jump_height;
        float jump_time;
        float jump_timer;

        // double jump
        size_t jump_charges;
        size_t jump_counter;
        size_t jump_cost;

        bool should_jump;
        bool is_jumping;
    };
    jump_values m_jump;

    float gravity;
    float max_fall;

    // collision qol
    float collision_forgiveness;
    float x_correction;

    struct glide_values
    {
        Timer glide_time;
        float max_glide_fall;
        float move_multiplier;
        bool is_gliding;
        bool can_glide;
        bool glide_unlocked;
    };
    glide_values m_glide;

    Vector2 starting_pawn_pos;
    Vector2 spawn_pos;

    Timer death_time;
    bool is_dead;

    // Spark stuff
    struct spark_values
    {
        size_t double_jump_unlock;
        size_t held;
    };
    spark_values m_spark;
};