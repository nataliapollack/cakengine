#include "Coordinator.hpp"
#include "raylib.h"
#include "raymath.h"

#include "Player.h"
#include "CollisionSystem.h"

#include "Core.h"

#include <iostream>
#include <limits>

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
                        Rectangle collision_box = GetCollisionRec(
                            collision_i.box, collision_j.box);

                        Event wall(Events::Collision::HIT_WALL);

                        wall.SetParam(Events::Collision::COLLISION_DATA, 
                            collision_box);

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