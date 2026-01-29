
#include "PlayerSystem.h"
#include "Coordinator.hpp"
#include "raylib.h"

#include "Player.h"
#include "Core.h"

extern Coordinator gCoordinator;

void PlayerSystem::init()
{

}

void PlayerSystem::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transform = gCoordinator.GetComponent<transform25D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);

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
        }
        if (IsKeyDown(KEY_UP))
        {
            transform.pos.y -= (playuh.speed * dt);
        }
    }
}