#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "PlayerSystem.h"

#include "Player.h"
#include "Core.h"

#include "ParticleEmitter.h"

#include <iostream>

extern Coordinator gCoordinator;

//constexpr float JUMP_SCALE = 0.11f;
constexpr float JUMP_SCALE = 0.03f;

void PlayerSystem::init()
{
    current_state = IDLE;
    starting_pawn_pos = Vector2{ -50, -50 };

    m_walk =
    {
        0.25f,  // time to accel to max speed
        0.15f,  // time to decel to zero
        400.0f, // max speed
        100.0f, // initial speed
        0.0f,   // how long player has been walking
        0.0f,   // direction player is walking
        true    // can walk
    };

    m_jump =
    {
        Timer(0.35f),   // coyote time
        Timer(0.1f),    // jump buffer time
        2.1f / 2.f, // left/right movement multiplier
        {},             // jump impulse (calculated later)
        { 2.25f, 1.5f }, // jump heights
        0.4f,           // jump time (used for tap vs hold jumping)
        0.0f,           // tracks time since jump started
        2,              // number of jumps
        2,              // number of jumps used since last reset
        1,              // cost of each jump
        false,          // has input and needs to execute
        false           // has started jump
    };

    m_glide =
    {
        Timer(1.0f), // length of glide
        1.0f,        // max fall vel while gliding
        1.0f,        // left/right movement multiplier
        false,       // has started glide
        false,       // able to glide (in air and moving down and has input)
        true         // progression stuff...
    };

    gravity = 6000.0f / 2.f;
    max_fall = 50.0f;

    collision_forgiveness = 0.4f;
    x_correction = 0.0f;

    for (size_t i = 0; i < m_jump.jump_height.size(); ++i)
    {
        m_jump.jump_impulse[i] =
            sqrtf(2.0f * gravity * JUMP_SCALE * m_jump.jump_height[i]);
    }

    death_time = Timer(1.0f);
    is_dead = false;

    m_spark = {
        5,
        0
    };

    for (auto& entity : entities_list)
    {
        auto& coll = gCoordinator.GetComponent<collidble>(entity);

        Vector2 base_offset{ coll.box.width / 2.0f, coll.box.height / 2.0f };

        Vector2 min_offset{ base_offset.x - 20.0f, base_offset.y - 20.0f };
        Vector2 max_offset{ base_offset.x + 20.0f, base_offset.y + 20.0f };

        // Particle jump thing
        gCoordinator.AddComponent(entity,
            particle_emitter{
                32,         // capacity
                0,           // alive count
                { min_offset, max_offset },
                ColorAlpha(WHITE, 0.7f), // color
                Vector2Rotate(Vector2UnitY, DEG2RAD * -60.0f),
                120.0f, // init dir
                { 350.0f, 600.0f },      // init speed
                {0.50f, 1.0f},        // init lifetime
                {5.0f, 10.0f}, // init size
                2,          // num per emit
                false ,        // emitting
                true,       // one shot effect
                COUNT,
                Timer(0.0f), // time between emits
                ET_JUMP,
                {}
            }
        );
    }


    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerSystem::HitWall));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::SPIKES, PlayerSystem::HitSpikes));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::SPAWNER, PlayerSystem::HitSpawner));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Item::PICKEDUP, PlayerSystem::PickedUpItem));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Item::DROPPEDOFF, PlayerSystem::DroppedItem));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::SPARK, PlayerSystem::HitSpark));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Input::START_DAEDALUS_FLIGHT,
            PlayerSystem::StopInput));
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Input::END_DAEDALUS_FLIGHT,
            PlayerSystem::StartInput));
}

void PlayerSystem::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        if (is_dead)
        {
            if (death_time.update(dt))
            {
                auto& transf = gCoordinator.GetComponent<transform2D>(entity);
                transf.pos = spawn_pos;

                auto& phy = gCoordinator.GetComponent<physics>(entity);

                phy.f = Vector2{ 0,0 };
                phy.vel = Vector2{ 0,0 };

                is_dead = false;
            }
        }
        else
        {
            current_state = IDLE;
            for (auto& entity : entities_list)
            {
                if (m_walk.can_walk)
                {
                    WalkInput();
                    JumpInput(entity, dt);
                    GlideInput(entity);
                }
            }

        }
        fixedUpdate(dt, entity);
        update_state();
    }
}

void PlayerSystem::fixedUpdate(float dt, Entity entity)
{
    AccumulateForces();

    auto& transf = gCoordinator.GetComponent<transform2D>(entity);
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);

    static float last_direction = 0.0f;

    auto& forces = phy.f;

    auto& vel = phy.vel;

    if (playuh.on_ground && vel.y > 5.0f
        && !m_jump.coyote_time.is_running())
    {
        m_jump.coyote_time.start();
        std::cout << vel.y << "\n";
    }

    if (m_jump.coyote_time.update(dt))
    {
        playuh.on_ground = false;
        //m_jump.jump_counter -= 1;
        m_jump.jump_cost = 2;
    }

    // Gliding
    if (m_glide.glide_time.update(dt))
        m_glide.can_glide = false;

    // Walking
    if (!FloatEquals(m_walk.direction, 0.0f)) {
        m_walk.time_walking = Clamp(
            m_walk.time_walking + dt,
            0.0f, m_walk.time_to_accel);

        last_direction = m_walk.direction;
    }
    else {
        m_walk.time_walking =
            Clamp(m_walk.time_walking - dt,
                0.0f, m_walk.time_to_decel);
    }

    float speed =
        Lerp(m_walk.min_speed, m_walk.max_speed, 
            m_walk.time_walking / m_walk.time_to_accel);

    if (FloatEquals(speed, m_walk.min_speed) && !m_glide.is_gliding)
        last_direction = 0.0f;

    float speed_modifier =
        (!playuh.on_ground) ? m_jump.move_multiplier : 1.0f;
    speed_modifier =
        (m_glide.is_gliding) ? m_glide.move_multiplier : speed_modifier;

    vel.x = last_direction * speed_modifier * speed * dt;

    vel.y += forces.y * dt * dt;
    if (m_glide.is_gliding)
    {
        vel.y = Clamp(vel.y, 0.0f, m_glide.max_glide_fall);
    }
    else if (vel.y > 0.0f)
    {
        vel.y = Clamp(vel.y, 0.0f, max_fall);
    }

    forces = Vector2Zero();

    transf.pos = Vector2Add(transf.pos, vel);

    float last_x = transf.pos.x;
    transf.pos.x = Lerp(transf.pos.x, transf.pos.x + x_correction, 0.5f);
    x_correction = Lerp(x_correction, 0.0f, 0.5f);
}

void PlayerSystem::WalkInput()
{
    m_walk.direction = 0.0f;
    if (IsKeyDown(KEY_LEFT))
    {
        m_walk.direction -= 1.0f;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        m_walk.direction += 1.0f;
    }
}

void PlayerSystem::JumpInput(Entity entity, float dt)
{
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);
    auto& emit = gCoordinator.GetComponent<particle_emitter>(entity);

    auto& vel = phy.vel;

    if (IsKeyPressed(KEY_SPACE))
    {
        m_jump.should_jump = true;
        m_jump.jump_buffering.start();
    }

    if (m_jump.should_jump && m_jump.jump_counter > 0)
    {
        emit.emitting = true;

        m_jump.should_jump = false;

        vel.y = -m_jump.jump_impulse.at(
            m_jump.jump_charges - m_jump.jump_counter );
        m_jump.jump_counter -= m_jump.jump_cost;
        m_jump.jump_cost = 1;

        playuh.on_ground = false;

        m_jump.is_jumping = true;
        m_jump.jump_timer = 0.0f;
    }

    if (m_jump.is_jumping) m_jump.jump_timer += dt;
    if (IsKeyReleased(KEY_SPACE))
    {
        if (m_jump.jump_timer < m_jump.jump_time)
        {
            vel.y /= 1.8f;
        }
    }

    if (m_jump.jump_buffering.update(dt))
        m_jump.should_jump = false;
}

void PlayerSystem::GlideInput(Entity entity)
{
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);

    auto& vel = phy.vel;

    m_glide.is_gliding = IsKeyDown(KEY_LEFT_SHIFT) && !playuh.on_ground &&
        vel.y > 0.0f && m_glide.can_glide && m_glide.glide_unlocked;

    if (IsKeyPressed(KEY_LEFT_SHIFT) && !m_glide.glide_time.is_running())
    {
        m_glide.glide_time.start();
        m_glide.can_glide = true;
    }
    if (IsKeyReleased(KEY_LEFT_SHIFT) && m_glide.glide_time.is_running())
    {
        m_glide.glide_time.stop();
    }
}

void PlayerSystem::HitWall(Event& event)
{
    for (auto& entity : entities_list)
    {
        Rectangle overlap =
            event.GetParam<Rectangle>(Events::Collision::COLLISION_DATA);

        auto& phy = gCoordinator.GetComponent<physics>(entity);
        auto& forces = phy.f;
        auto& vel = phy.vel;

        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);

        auto& coll = gCoordinator.GetComponent<collidble>(entity);

        if (overlap.height <= overlap.width)
        {
            if (vel.y > 0 && overlap.y > transf.pos.y) // floor
            {
                transf.pos.y -= overlap.height;
                vel.y = 0.0f;

                playuh.on_ground = true;
                m_jump.jump_counter = m_jump.jump_charges;

                m_jump.is_jumping = false;
                m_jump.jump_cost = 1;
            }
            else if (vel.y < 0) // roof
            {
                // collision forgiveness
                if (overlap.width < coll.box.width * collision_forgiveness)
                {
                    if (overlap.x > transf.pos.x)
                        x_correction = -overlap.width;
                    else
                        x_correction = overlap.width;
                }
                else // or not
                {
                    transf.pos.y += overlap.height;
                    vel.y = 0.0f;
                }
            }
        }
        else
        {
            if (vel.x > 0)
            {
                transf.pos.x -= overlap.width;
                vel.x = 0.0f;
            }
            else if (vel.x < 0)
            {
                transf.pos.x += overlap.width;
                vel.x = 0.0f;
            }
        }
    }
}

void PlayerSystem::AccumulateForces()
{
    for (auto& entity : entities_list)
    {
        auto& phy = gCoordinator.GetComponent<physics>(entity);
        auto& forces = phy.f;
        auto& vel = phy.vel;

        auto& playuh = gCoordinator.GetComponent<player>(entity);

        if (!playuh.on_ground && IsKeyDown(KEY_SPACE) &&
            vel.y < 0.0f && vel.y > -1.0f)
        {
            forces.y += gravity / 2.0f;
        }
        else if (m_glide.is_gliding)
        {
            forces.y += gravity / 4.0f;
        }
        else
        {
            forces.y += gravity;
        }
    }
}

void PlayerSystem::PickedUpItem(Event& event)
{
    for (auto& entity : entities_list)
    {
        auto& playuh = gCoordinator.GetComponent<player>(entity);

        OBJECT_TYPE id = event.GetParam<OBJECT_TYPE>(
            Events::Item::PickedUp::OBJTYPE);

        playuh.holding = TEMP;
    }

}

void PlayerSystem::DroppedItem(Event& event)
{
    for (auto& entity : entities_list)
    {
        auto& playuh = gCoordinator.GetComponent<player>(entity);


        playuh.holding = NONE;
    }
}

void PlayerSystem::ResetPlayerPos()
{
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& phy = gCoordinator.GetComponent<physics>(entity);

        phy.f = Vector2{ 0,0 };
        phy.vel = Vector2{ 0,0 };
        transf.pos = starting_pawn_pos;
    }
}

void PlayerSystem::SetGlideUnlocked(bool value)
{
    m_glide.glide_unlocked = value;
}

void PlayerSystem::SetDoubleJumpUnlocked(bool value)
{
    if (value)
        m_jump.jump_charges = 2;
    else
        m_jump.jump_charges = 1;
}

void PlayerSystem::HitSpikes(Event& event)
{
    // note to nat. maybe add delay + player state here.
    for (auto& entity : entities_list)
    {
        if (is_dead) continue;

        current_state = FALL;

        m_walk.direction = 0.0f;

        death_time.start();
        is_dead = true;
    }
}

void PlayerSystem::HitSpawner(Event& event)
{
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        spawn_pos = transf.pos;
    }
}

void PlayerSystem::update_state()
{
    for (auto& entity : entities_list)
    {
        auto& rend = gCoordinator.GetComponent<render_environment>(entity);
        rend.txt = (ASSETS)current_state;
    }
}

void PlayerSystem::HitSpark(Event& event)
{
    //std::cout << "Spark hit\n";
    m_spark.held += 1;

    if (m_spark.held >= m_spark.double_jump_unlock)
        SetDoubleJumpUnlocked(true);
}

void PlayerSystem::StartInput(Event& event)
{
    m_walk.can_walk = true;
}

void PlayerSystem::StopInput(Event& event)
{
    m_walk.can_walk = false;
    m_walk.direction = 0.0f;
}
