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

                if (stats_j.type != ITEM)
                {
                    collision_j.box.x = transform_j.pos.x;
                    collision_j.box.y = transform_j.pos.y;
                }

                if (stats_j.type == WAYPOINT)
                {
                    collision_j.box.x = transform_j.pos.x - collision_j.box.width / 2.0f;
                    collision_j.box.y = transform_j.pos.y - collision_j.box.height / 2.0f;
                }

                {
                    collision_i.box.x = transform_i.pos.x + 25;
                    collision_i.box.y = transform_i.pos.y;
                }

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
                        Event item(Events::Item::PICKEDUP);
                        item.SetParam(Events::Item::PickedUp::ITEMID, j);
                        // tmep until we get a list of all items ig
                        item.SetParam(Events::Item::PickedUp::OBJTYPE, stats_j.type);

                        gCoordinator.SendEvent(item);
                    }

                    // this is for items handling, you can ignore how i did this if you wanna do something quirky
                    if (stats_j.type == DROPOFF)
                    {
                        auto& playuh = gCoordinator.GetComponent<player>(i);
                        Event item(Events::Item::DROPPEDOFF);
                        item.SetParam(Events::Item::DroppedOff::ITEMID, playuh.holding);
                        item.SetParam(Events::Item::DroppedOff::OBJECTID, j);

                        gCoordinator.SendEvent(item);
                    }

                    if (stats_j.type == SPIKES)
                    {
                        Event ouch(Events::Collision::SPIKES);
                        gCoordinator.SendEvent(ouch);
                    }

                    if (stats_j.type == SPAWNER)
                    {
                        Event spawner(Events::Collision::SPAWNER);
                        gCoordinator.SendEvent(spawner);
                    }

                    if (stats_j.type == SPARK)
                    {
                        Event spark(Events::Collision::SPARK);
                        spark.SetParam(Events::Spark::Collected::SPARKID, j);
                        gCoordinator.SendEvent(spark);
                    }

                    if (stats_j.type == WAYPOINT)
                    {
                        Event waypoint(Events::Collision::WAYPOINT);

                        waypoint.SetParam(Events::Collision::WAYPOINT_DATA, j);

                        gCoordinator.SendEvent(waypoint);
                    }

                    if (stats_j.type == ENDPOINT)
                    {
                        Event endpoint(Events::Collision::ENDPOINT);

                        gCoordinator.SendEvent(endpoint);
                    }

                    if (stats_j.type == WAITINGPOINT)
                    {
                        Event waiting(Events::Collision::WAITINGGAME);

                        gCoordinator.SendEvent(waiting);
                    }
                }
            }
        }
    }
}

void CollisionSystem::debug_draw_collisions()
{
    for (auto i : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(i);

        if (stats.active)
        {
            auto& collision = gCoordinator.GetComponent<collidble>(i);
            DrawRectangleRec(collision.box, ColorAlpha(RED, 0.3));

            DrawText(TextFormat("EN: %d", i), collision.box.x, collision.box.y, 10, GREEN);
        }
    }
}