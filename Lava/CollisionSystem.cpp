#include "CollisionSystem.h"
#include "raylib.h"
#include "Coordinator.hpp"

#include "Core.h"
#include "Player.h"

extern Coordinator gCoordinator;

void CollisionSystem::CheckCollisions()
{
    for (auto& entity_i : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity_i);

        if (gCoordinator.HasComponent<player>(entity_i))
        {
            for (auto& entity_j : entities_list)
            {
                if (entity_i == entity_j)
                {
                    continue;
                }

                auto& stats_j = gCoordinator.GetComponent<status>(entity_j);

                if (!stats_j.active)
                    continue;

                auto& collision_i = gCoordinator.GetComponent<collidble>(entity_i);
                auto& collision_j = gCoordinator.GetComponent<collidble>(entity_j);

                auto& transform_i = gCoordinator.GetComponent<transform2D>(entity_i);
                auto& transform_j = gCoordinator.GetComponent<transform2D>(entity_j);

                if (CheckCollisionRecs(collision_i.box, collision_j.box))
                {
                    if (stats_j.type == WALL)
                    {
                        Rectangle collision_box = GetCollisionRec(
                            collision_i.box, collision_j.box);

                        Event wall(Events::Collision::HIT_WALL);

                        wall.SetParam(Events::Collision::COLLISION_DATA,
                            collision_box);

                        gCoordinator.SendEvent(wall);
                    }
                }
            }
        }
    }
}

void CollisionSystem::DebugDrawBoxes()
{
    for (auto& entity : entities_list)
    {
        auto& collision_box = gCoordinator.GetComponent<collidble>(entity);

        DrawRectangleRec(collision_box.box, ColorAlpha(RED, 0.75));
    }
}