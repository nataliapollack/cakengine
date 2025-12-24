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
    time_walking = 0.0f;
    time_to_accel = 2.5f;
    time_to_decel = 0.25f;
    max_speed = 300.0f;
    min_speed = 100.0f;

    jump_impulse = 10.0f;
    gravity = 980.f;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerSystem::HitWall));
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

        float direction = 0.0f;
        static float last_direction = 0.0f;

        auto& forces = phy.f;
        std::cout << forces.y << "\n";

        if (IsKeyDown(KEY_LEFT))
        {
            direction -= 1.0f;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            direction += 1.0f;
        }

        auto& vel = phy.vel;

        if (IsKeyPressed(KEY_SPACE) && playuh.on_ground)
        {
            vel.y = -jump_impulse;
            playuh.on_ground = false;
        }

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

        vel.x = last_direction *
            Lerp(min_speed, max_speed, time_walking) * dt;

        vel.y += forces.y * dt * dt;

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

        auto& playuh = gCoordinator.GetComponent<player>(entity);

        if (!playuh.on_ground)
            forces.y += gravity;
    }
}
