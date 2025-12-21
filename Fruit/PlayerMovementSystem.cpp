#include "PlayerMovementSystem.h"

#include "Coordinator.hpp"
#include "raylib.h"
#include "Core.h"
#include "Player.h"
#include "Events.hpp"

#define GRAVITY 100
#define FLOOR 400

extern Coordinator gCoordinator;

void PlayerMovement::init()
{
    can_jump = true;
    current_jump_spd = 0.0f;
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Obstacles::OBST, PlayerMovement::TriggerObstacleHit));
}

void PlayerMovement::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);

        if (IsKeyDown(KEY_LEFT))
        {
            transf.pos.x -= (playuh.speed * dt);

            if (transf.pos.x < 0.0f)
            {
                transf.pos.x = 0;
            }
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transf.pos.x += (playuh.speed * dt);

            if (transf.pos.x > GetScreenWidth() + 100)
            {
                transf.pos.x = GetScreenWidth() + 100;
            }
        }
        //if (IsKeyDown(KEY_SPACE) && can_jump)
        //{
        //    current_jump_spd = -playuh.jump_height;
        //    can_jump = false;
        //}

        //if (!transf.pos.y < FLOOR)
        //{
        //    transf.pos.y += current_jump_spd * dt;
        //    current_jump_spd += GRAVITY * dt;

        //    if (transf.pos.y >= FLOOR)
        //    {
        //        current_jump_spd = 0;
        //        transf.pos.y = FLOOR;
        //        can_jump = true;
        //    }
        //}
        //else
        //{
        //    can_jump = false;
        //}

        playuh.speed += 1.0f;
    }
}

void PlayerMovement::TriggerObstacleHit(Event& event)
{
    for (auto& entity : entities_list)
    {
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        playuh.speed = 100.0f;
    }
}