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
    jump_height = 100.0f;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerSystem::HitWall));
}

void PlayerSystem::update(float dt)
{
    AccumulateForces();
    std::cout << forces.y << "\n";
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);

        float direction = 0.0f;
        static float last_direction = 0.0f;

        if (FloatEquals(forces.y, 0.0f))
        {
            playuh.on_ground = true;
        }
        else
        {
            playuh.on_ground = false;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            direction -= 1.0f;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            direction += 1.0f;
        }

        if (IsKeyPressed(KEY_SPACE) && playuh.on_ground)
        {
            v.y = -jump_impulse;
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

        v.x = last_direction *
            Lerp(min_speed, max_speed, time_walking) * dt;

        v.y += forces.y * dt * dt;

        forces = Vector2Zero();

        transf.pos = Vector2Add(transf.pos, v);
    }
}

void PlayerSystem::HitWall(Event& event)
{
    //Vector2 impulse = 
    //    event.GetParam<Vector2>(Events::Collision::IMPULSE_FORCE);

    //forces = Vector2Add(forces, impulse);
    forces.y += -gravity;
    v.y = 0.0f;

    //std::cout << "Here\n";
}

void PlayerSystem::AccumulateForces()
{
    forces.y += gravity;
}
