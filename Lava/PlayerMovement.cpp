#include "PlayerMovement.h"

#include "Coordinator.hpp"
#include "raylib.h"

#include "Core.h"
#include "Player.h"

extern Coordinator gCoordinator;

void PlayerMovementSystem::move_player(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transform = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        auto& stats = gCoordinator.GetComponent<status>(entity);

        if (IsKeyDown(KEY_LEFT))
        {
            transform.pos.x -= (playuh.speed * dt);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transform.pos.x += (playuh.speed * dt);
        }
        if (IsKeyDown(KEY_DOWN))
        {
            transform.pos.y += (playuh.speed * dt);
            stats.dirty = true;
        }
        if (IsKeyDown(KEY_UP))
        {
            transform.pos.y -= (playuh.speed * dt);
            stats.dirty = true;
        }
    }
}