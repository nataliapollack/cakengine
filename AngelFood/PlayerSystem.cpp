#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "PlayerSystem.h"

#include "Player.h"
#include "Core.h"

#include <iostream>

extern Coordinator gCoordinator;

constexpr float JUMP_SCALE = 0.045f;

void PlayerSystem::init()
{
    spawn_pos = Vector2{ 210, 270 };

    m_walk =
    {
        1.0f,
        0.25f,
        500.0f,
        300.0f,
        0.0f
    };

    m_jump =
    {
        Timer(0.35f),
        Timer(0.1f),
        {},
        { 3.0f, 1.5f },
        0.4f,
        0.0f,
        2, // change to 1 to disable double jump
        2,
        false,
        false
    };

    m_glide =
    {
        Timer(3.0f),
        1.0f,
        false,
        false,
        true
    };

    gravity = 3000.0f;
    max_fall = 50.0f;

    collision_forgiveness = 0.4f;
    x_correction = 0.0f;

    for (size_t i = 0; i < m_jump.jump_height.size(); ++i)
    {
        m_jump.jump_impulse[i] =
            sqrtf(2.0f * gravity * JUMP_SCALE * m_jump.jump_height[i]);
    }

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerSystem::HitWall));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Item::PICKEDUP, PlayerSystem::PickedUpItem));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Item::DROPPEDOFF, PlayerSystem::DroppedItem));

    m_time = std::make_unique<TimeManager>();
}

void PlayerSystem::update(float dt)
{
    m_time->increment();

    for (auto& entity : entities_list)
    {
        WalkInput();
        JumpInput(entity);
        GlideInput(entity);
    }

    while (m_time->needsFixedUpdate())
    {
        fixedUpdate();
    }
}

void PlayerSystem::fixedUpdate()
{
    AccumulateForces();

    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        auto& phy = gCoordinator.GetComponent<physics>(entity);

        static float last_direction = 0.0f;

        auto& forces = phy.f;

        auto& vel = phy.vel;

        if (playuh.on_ground && vel.y > 0.0f
            && !m_jump.coyote_time.is_running())
        {
            m_jump.coyote_time.start();
        }

        if (m_jump.coyote_time.update(m_time->getFixedDt()))
        {
            playuh.on_ground = false;
        }

        // Gliding
        if (IsKeyPressed(KEY_LEFT_SHIFT) && !m_glide.glide_time.is_running())
        {
            m_glide.glide_time.start();
            m_glide.can_glide = true;
        }
        if (IsKeyReleased(KEY_LEFT_SHIFT) && m_glide.glide_time.is_running())
        {
            m_glide.glide_time.stop();
        }

        if (m_glide.glide_time.update(m_time->getFixedDt()))
            m_glide.can_glide = false;

        // Walking
        if (!FloatEquals(m_walk.direction, 0.0f)) {
            m_walk.time_walking = Clamp(
                m_walk.time_walking + m_time->getFixedDt(), 
                0.0f, m_walk.time_to_accel);

            last_direction = m_walk.direction;
        }
        else {
            m_walk.time_walking = 
                Clamp(m_walk.time_walking - m_time->getFixedDt(), 
                0.0f, m_walk.time_to_decel);
        }

        float speed = 
            Lerp(m_walk.min_speed, m_walk.max_speed, m_walk.time_walking);

        if (FloatEquals(speed, m_walk.min_speed) && !m_glide.is_gliding)
            last_direction = 0.0f;

        float speed_modifier = (!playuh.on_ground) ? 1.5f : 1.0f;

        vel.x = last_direction * speed_modifier * speed * m_time->getFixedDt();

        vel.y += forces.y * m_time->getFixedDt() * m_time->getFixedDt();
        if (m_glide.is_gliding)
        {
            vel.y = Clamp(vel.y, 0.0f, m_glide.max_glide_fall);
        }
        else if (vel.y > 0.0f)
        {
            std::cout << vel.y << "\n";
            vel.y = Clamp(vel.y, 0.0f, max_fall);
        }

        forces = Vector2Zero();

        transf.pos = Vector2Add(transf.pos, vel);

        float last_x = transf.pos.x;
        transf.pos.x = Lerp(transf.pos.x, transf.pos.x + x_correction, 0.5f);
        x_correction = Lerp(x_correction, 0.0f, 0.5f);
    }
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

void PlayerSystem::JumpInput(Entity entity)
{
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);

    auto& vel = phy.vel;

    if (IsKeyPressed(KEY_SPACE))
    {
        m_jump.should_jump = true;
        m_jump.jump_buffering.start();
    }

    if (m_jump.should_jump &&
        (playuh.on_ground || m_jump.jump_counter == (m_jump.jump_charges - 1)))
    {
        m_jump.should_jump = false;

        vel.y = -m_jump.jump_impulse.at(
            m_jump.jump_charges - m_jump.jump_counter );
        m_jump.jump_counter -= 1;

        playuh.on_ground = false;

        m_jump.is_jumping = true;
        m_jump.jump_timer = 0.0f;
    }

    if (m_jump.is_jumping) m_jump.jump_timer += m_time->getFixedDt();
    if (IsKeyReleased(KEY_SPACE))
    {
        if (m_jump.jump_timer < m_jump.jump_time)
        {
            vel.y /= 1.5f;
        }
    }

    if (m_jump.jump_buffering.update(m_time->getFixedDt()))
        m_jump.should_jump = false;
}

void PlayerSystem::GlideInput(Entity entity)
{
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);

    auto& vel = phy.vel;

    m_glide.is_gliding = IsKeyDown(KEY_LEFT_SHIFT) && !playuh.on_ground &&
        vel.y > 0.0f && m_glide.can_glide && m_glide.glide_unlocked;
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
            }
            else if (vel.y < 0) // roof
            {
                // collision forgiveness
                if (overlap.width < coll.box.width * collision_forgiveness)
                {
                    if (overlap.x > transf.pos.x)
                        x_correction = -overlap.width;
                        //transf.pos.x -= overlap.width;
                    else
                        x_correction = overlap.width;
                        //transf.pos.x += overlap.width;
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
    auto& playuh = gCoordinator.GetComponent<player>(0);

    OBJECT_TYPE id =  event.GetParam<OBJECT_TYPE>(
        Events::Item::PickedUp::OBJTYPE);

    playuh.holding = TEMP;

}

void PlayerSystem::DroppedItem(Event& event)
{

    auto& playuh = gCoordinator.GetComponent<player>(0);


    playuh.holding = NONE;
}

void PlayerSystem::ResetPlayerPos()
{
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& phy = gCoordinator.GetComponent<physics>(entity);

        phy.f = Vector2{ 0,0 };
        phy.vel = Vector2{ 0,0 };
        transf.pos = spawn_pos;
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

// Time Manager  -------------------------------------------------------------

using namespace std::chrono;

TimeManager::TimeManager()
    : m_lastTime(steady_clock::now()), m_currTime(steady_clock::now()),
      m_timeTaken(), m_accumulator(0.0f), m_time(0.0f), m_deltaTime(0.0f) {}

void TimeManager::increment()
{
    m_currTime = steady_clock::now();
    m_timeTaken = m_currTime - m_lastTime;
    m_deltaTime = static_cast<float>(m_timeTaken.count()) *
        steady_clock::period::num / steady_clock::period::den;
    m_lastTime = m_currTime;
    m_accumulator += m_deltaTime;
}

bool TimeManager::needsFixedUpdate() {
    const bool result = m_accumulator >= m_fixedDt;

    if (result) {
        m_accumulator -= m_fixedDt;
        m_time += m_fixedDt;
    }

    return result;
}

void TimeManager::resetLastTime() { m_lastTime = steady_clock::now(); }

const float TimeManager::getFixedDt() const { return m_fixedDt; }

const float TimeManager::getDeltaTime() const { return m_deltaTime; }

const float TimeManager::getTotalTime() const { return m_time; }
