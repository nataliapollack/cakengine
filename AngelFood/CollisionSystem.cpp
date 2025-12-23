#include "Coordinator.hpp"
#include "raylib.h"

#include "CollisionSystem.h"

#include "Core.h"

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

                auto& stats_j = gCoordinator.GetComponent<status>(i);

                if (!stats_j.active)
                    continue;


                auto& collision_j = gCoordinator.GetComponent<collidble>(j);
                auto& collision_i = gCoordinator.GetComponent<collidble>(i);

                auto& transform_i = gCoordinator.GetComponent<transform2D>(i);
                auto& transform_j = gCoordinator.GetComponent<transform2D>(j);

                if (CheckCollisionRecs(collision_i.box, collision_j.box))
                {
                    // idk whatever here

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