#include "PlayerMovement.h"

#include "Coordinator.hpp"
#include "raylib.h"

#include "Core.h"
#include "Player.h"
#include "AssetManager.hpp"
#include "Viewport3D.hpp"

extern Coordinator gCoordinator;

void PlayerMovementSystem::init()
{
    collision_forgiveness = 0.4f;
    x_correction = 0.0f;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_WALL, PlayerMovementSystem::HitWall));

}

void PlayerMovementSystem::move_player(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transform = gCoordinator.GetComponent<transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<player>(entity);
        auto& stats = gCoordinator.GetComponent<status>(entity);

        bool moved = false;

        if (IsKeyDown(KEY_LEFT))
        {
            transform.pos.x -= (playuh.speed.x * dt);
            moved = true;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transform.pos.x += (playuh.speed.x * dt);
            moved = true;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            transform.pos.y += (playuh.speed.y * dt);
            stats.dirty = true;
            moved = true;
        }
        if (IsKeyDown(KEY_UP))
        {
            transform.pos.y -= (playuh.speed.y * dt);
            stats.dirty = true;
            moved = true;
        }

        /*auto& model = gCoordinator.GetComponent<model_view>(entity);
        auto& modelAnim = gCoordinator.GetComponent<ModelAnimationInfo>(entity);

        if (moved) {
            modelAnim.curFrame++;
            if (modelAnim.curFrame >= modelAnim.pData[0].frameCount)
                modelAnim.curFrame = 0;

            UpdateModelAnimation(*model.model, modelAnim.pData[0], modelAnim.curFrame);
        }*/
    }
}

void PlayerMovementSystem::HitWall(Event& event)
{
    for (auto& entity : entities_list)
    {
        Rectangle overlap =
            event.GetParam<Rectangle>(Events::Collision::COLLISION_DATA);

        auto& playuh = gCoordinator.GetComponent<player>(entity);
        float forces = 10;
        auto& vel = playuh.speed;

        auto& transf = gCoordinator.GetComponent<transform2D>(entity);

        auto& coll = gCoordinator.GetComponent<collidble>(entity);

        if (overlap.height <= overlap.width)
        {
            if (vel.y > 0 && overlap.y > transf.pos.y) // floor
            {
                transf.pos.y -= overlap.height;
                vel.y = 0.0f;
            }
            else if (vel.y < 0) // roof
            {
                // collision forgiveness 
                if (overlap.width < coll.box.width * collision_forgiveness)
                {
                    if (overlap.x > transf.pos.x)
                        x_correction = -overlap.width;
                    else
                        x_correction = overlap.width;
                }
                else // or not
                {
                    transf.pos.y += overlap.height;
                    vel.y = 0.0f;
                }
            }
        }
        else
        {
            if (vel.x > 0)
            {
                transf.pos.x -= overlap.width;
                vel.x = 0.0f;
            }
            else if (vel.x < 0)
            {
                transf.pos.x += overlap.width;
                vel.x = 0.0f;
            }
        }
    }
}
