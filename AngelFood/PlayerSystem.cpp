#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "PlayerSystem.h"

#include "Player.h"
#include "Core.h"

#include <iostream>

extern Coordinator gCoordinator;

void PlayerSystem::init()
{
    coyote_time = Timer(0.35f);
    jump_buffering = Timer(0.1f);

    time_walking = 0.0f;
    time_to_accel = 1.0f;
    time_to_decel = 0.25f;
    max_speed = 400.0f;
    min_speed = 100.0f;

    jump_impulse = 10.0f;
    gravity = 980.f;

    max_glide_fall = 1.0f;

    jump_charges = 2;

    should_jump = false;
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
   // std::cout << forces.y << "\n";
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        auto& phy = gCoordinator.GetComponent<physics>(entity);

        float direction = 0.0f;
        static float last_direction = 0.0f;

        auto& forces = phy.f;
        //std::cout << forces.y << "\n";

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
        if (IsKeyPressed(KEY_SPACE))
        {
            should_jump = true;
            jump_buffering.start();
        }
        if (should_jump && 
            (playuh.on_ground || jump_counter == (jump_charges - 1 )))
        {
            should_jump = false;

            jump_counter -= 1;

            vel.y = -jump_impulse;
            playuh.on_ground = false;
        }
        if (jump_buffering.update(dt))
            should_jump = false;

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