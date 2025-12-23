#include "Coordinator.hpp"
#include "raylib.h"

#include "PlayerSystem.h"

#include "Player.h"
#include "Core.h"

extern Coordinator gCoordinator;

void PlayerSystem::init()
{
    jump_height = 100.0f;
}

void PlayerSystem::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);

        if (IsKeyDown(KEY_LEFT))
        {
            transf.pos.x -= (playuh.vx * dt);

        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transf.pos.x += (playuh.vx * dt);
        }
    }
}