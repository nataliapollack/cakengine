#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "Player.h"
#include "CollisionSystem.h"

#include "Core.h"

#include <iostream>

extern Coordinator gCoordinator;

void CollisionSystem::CheckCollisions()
{
    for (auto i : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(i);

        if (stats.active)
        {
            if (stats.type != PLAYER)
                continue;

            for (auto j : entities_list)
            {
                if (i == j)
                    continue;

                auto& stats_j = gCoordinator.GetComponent<status>(j);

                if (!stats_j.active)
                    continue;


                auto& collision_i = gCoordinator.GetComponent<collidble>(i);
                auto& collision_j = gCoordinator.GetComponent<collidble>(j);

                auto& transform_i = gCoordinator.GetComponent<transform2D>(i);
                auto& transform_j = gCoordinator.GetComponent<transform2D>(j);

                collision_i.box.x = transform_i.pos.x;
                collision_i.box.y = transform_i.pos.y;

                collision_j.box.x = transform_j.pos.x;
                collision_j.box.y = transform_j.pos.y;

                if (CheckCollisionRecs(collision_i.box, collision_j.box))
                {
                    // idk whatever here

                    if (stats_j.type == WALL)
                    {
                        Event wall(Events::Collision::HIT_WALL);

                        Vector2 impulse_force = Vector2Normalize(
                            Vector2Subtract(transform_j.pos, transform_i.pos)
                        );

                        wall.SetParam(Events::Collision::IMPULSE_FORCE, 
                            impulse_force);

                        gCoordinator.SendEvent(wall);
                    }

                    // this is for items handling, you can ignore how i did this if you wanna do something quirky
                    if (stats_j.type == ITEM)
                    {
                        Event item(Events::Item::PICKED_UP);
                        item.SetParam(Events::Item::OBJ_TYPE, j);

                        gCoordinator.SendEvent(item);
                    }
                }
            }
        }
    }
}