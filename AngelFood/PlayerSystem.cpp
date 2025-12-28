#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "PlayerSystem.h"

#include "Player.h"
#include "Core.h"

#include <iostream>

extern Coordinator gCoordinator;

constexpr float JUMP_SCALE = 0.03f;

void PlayerSystem::init()
{
    spawn_pos = Vector2{ 210, 270 };

    coyote_time = Timer(0.35f);
    jump_buffering = Timer(0.1f);

    time_walking = 0.0f;
    time_to_accel = 1.0f;
    time_to_decel = 0.25f;
    max_speed = 400.0f;
    min_speed = 100.0f;

    gravity = 3000.f;

    //jump_impulse = 01.0f;
    jump_height = { 2.5f, 1.5f };
    jump_time = 0.4f;

    for (size_t i = 0; i < jump_height.size(); ++i)
    {
        jump_impulse[i] = sqrtf(2.0f * gravity * JUMP_SCALE * jump_height[i]);
    }

    max_glide_fall = 1.0f;

    jump_charges = 2;

    should_jump = false;
    is_jumping = false;
    is_gliding = false;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerSystem::HitWall));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Item::PICKEDUP, PlayerSystem::PickedUpItem));

    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::DROPPEDOFF, PlayerSystem::DroppedItem));
}

void PlayerSystem::update(float dt)
{
    AccumulateForces();

    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        auto& phy = gCoordinator.GetComponent<physics>(entity);
        auto& BADDDDD = gCoordinator.GetComponent<render>(entity);
        auto& BADDDDD2 = gCoordinator.GetComponent<collidble>(entity);

        float direction = 0.0f;
        static float last_direction = 0.0f;

        auto& forces = phy.f;

        if (IsKeyDown(KEY_LEFT))
        {
            direction -= 1.0f;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            direction += 1.0f;
        }

        auto& vel = phy.vel;

        if (playuh.on_ground && vel.y > 0.0f && !coyote_time.is_running())
        {
            coyote_time.start();
        }

        if (coyote_time.update(dt))
        {
            playuh.on_ground = false;
        }

        // Gliding
        is_gliding = IsKeyDown(KEY_LEFT_SHIFT) && !playuh.on_ground && 
            vel.y > 0.0f;

        // Jumping
        ProcessJump(entity, dt);

        if (!FloatEquals(direction, 0.0f)) {
            time_walking = Clamp(time_walking + dt, 0.0f, time_to_accel);

            last_direction = direction;
        }
        else {
            time_walking = Clamp(time_walking - dt, 0.0f, time_to_decel);
        }

        float speed = Lerp(min_speed, max_speed, time_walking);

        if (FloatEquals(speed, min_speed))
            last_direction = 0.0f;

        float speed_modifier = (is_gliding) ? 1.5f : 1.0f;

        vel.x = last_direction * speed_modifier *
            Lerp(min_speed, max_speed, time_walking) * dt;

        vel.y += forces.y * dt * dt;
        if (is_gliding)
        {
            vel.y = Clamp(vel.y, 0.0f, max_glide_fall);
        }

        forces = Vector2Zero();

        transf.pos = Vector2Add(transf.pos, vel);
    }
}

void PlayerSystem::ProcessJump(Entity entity, float dt)
{
    auto& playuh = gCoordinator.GetComponent<player>(entity);
    auto& phy = gCoordinator.GetComponent<physics>(entity);

    auto& vel = phy.vel;

    if (IsKeyPressed(KEY_SPACE))
    {
        should_jump = true;
        jump_buffering.start();
    }

    if (should_jump &&
        (playuh.on_ground || jump_counter == (jump_charges - 1)))
    {
        should_jump = false;

        vel.y = -jump_impulse.at( jump_charges - jump_counter );
        jump_counter -= 1;

        playuh.on_ground = false;

        is_jumping = true;
        jump_timer = 0.0f;
    }

    if (is_jumping) jump_timer += dt;
    if (IsKeyReleased(KEY_SPACE))
    {
        if (jump_timer < jump_time)
        {
            vel.y /= 2.0f;
        }
    }

    if (jump_buffering.update(dt))
        should_jump = false;
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

        if (overlap.height < overlap.width)
        {
            if (vel.y > 0)
            {
                transf.pos.y -= overlap.height;
                vel.y = 0.0f;

                playuh.on_ground = true;
                jump_counter = jump_charges;

                is_jumping = false;
            }
            else if (vel.y < 0)
            {
                transf.pos.y += overlap.height;
                vel.y = 0.0f;
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
        else if (is_gliding)
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