
#include "PlayerMovementSystem.h"
#include "Coordinator.hpp"
#include "raylib.h"

#include "Transform.h"
#include "Player.h"

extern Coordinator gCoordinator;

void PlayerMovementSystem::init()
{

}

void PlayerMovementSystem::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<Player>(entity);

        if (IsKeyDown(KEY_LEFT))
        {
            transform.position.x -= (playuh.speed * dt);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transform.position.x += (playuh.speed * dt);
        }
        //if (IsKeyDown(KEY_DOWN))
        //{
        //    transform.position.y += (playuh.speed * dt);
        //}
        //if (IsKeyDown(KEY_UP))
        //{
        //    transform.position.y -= (playuh.speed * dt);
        //}
    }
}