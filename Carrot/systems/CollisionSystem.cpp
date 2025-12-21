#include "CollisionSystem.h"
#include "Coordinator.hpp"
#include "Events.hpp"
#include "Game.h"


#include "components/Collidble.h"
#include "components/Transform.h"

extern Coordinator gCoordinator;

void CollisionSystem::CheckCollisions()
{
    for (auto i : entities_list)
    {
        auto& collision_i = gCoordinator.GetComponent<Collidible>(i);
//        auto& transform_i = gCoordinator.GetComponent<Transform2D>(i);

        if (collision_i.type != PLAYER)
            continue;

        for (auto j : entities_list)
        {
            if (i == j)
                continue;

            auto& collision_j = gCoordinator.GetComponent<Collidible>(j);

            auto& transform_i = gCoordinator.GetComponent<Transform2D>(i);
            auto& transform_j = gCoordinator.GetComponent<Transform2D>(j);

            Rectangle box_i = { transform_i.position.x, transform_i.position.y, collision_i.collision_box.x,collision_i.collision_box.y };
            Rectangle box_j = { transform_j.position.x, transform_j.position.y, collision_j.collision_box.x,collision_j.collision_box.y };

            if (CheckCollisionRecs(box_i, box_j) && IsKeyPressed(KEY_ENTER))
            {
                if (collision_j.type == DOOR || collision_j.type == LOCKEDDOOR || collision_j.type == EXITDOOR)
                {
                    Event coll_door(Events::Confirmation::CONFIRM);
                    coll_door.SetParam(Events::Confirmation::CONFIRM, (int)collision_j.type);

                    gCoordinator.SendEvent(coll_door);
                }
                else
                {
                    Event collision_happen(Events::Collision::OBJ_TYPE);
                    collision_happen.SetParam(Events::Collision::OBJ_TYPE, collision_j.type);
                    gCoordinator.SendEvent(collision_happen);

                }
            }
        }
    }
}