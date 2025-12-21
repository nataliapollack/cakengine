#include "CollisionSystem.h"
#include "Coordinator.hpp"
#include "Events.hpp"
#include "Game.h"


#include "Core.h"
#include "Obstacles.h"

extern Coordinator gCoordinator;

void CollisionSystem::CheckCollisions()
{
    for (auto i : entities_list)
    {
        auto& collision_i = gCoordinator.GetComponent<collidible>(i);

        if (collision_i.type != PLAYER)
            continue;

        for (auto j : entities_list)
        {
            if (i == j)
                continue;

            auto& collision_j = gCoordinator.GetComponent<collidible>(j);

            auto& transform_i = gCoordinator.GetComponent<transform2>(i);
            auto& transform_j = gCoordinator.GetComponent<transform2>(j);

            Rectangle box_i = { transform_i.pos.x, transform_i.pos.y , collision_i.size.x,collision_i.size.y};
            Rectangle box_j = { transform_j.pos.x , transform_j.pos.y, collision_j.size.x,collision_j.size.y };

            if (CheckCollisionRecs(box_i, box_j))
            {
                if (collision_j.type == OBSTACLE )
                {
                    Event coll_door(Events::Obstacles::OBST);

                    gCoordinator.SendEvent(coll_door);
                }
                if (collision_j.type == FLAG)
                {
                    Event collision_happen(Events::Obstacles::FLAGS);
                    gCoordinator.SendEvent(collision_happen);

                }
                if (collision_j.type == FINISH_LINE)
                {
                    Event collision_happen(Events::Obstacles::FINISH);
                    gCoordinator.SendEvent(collision_happen);
                }
            }
        }
    }
}