#include "CameraSystem.h"

#include "Coordinator.hpp"
#include "Core.h"

extern Coordinator gCoordinator;

void CameraSystem::init()
{
    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    state = FOLLOW_PLAYER;

    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        camera.target = Vector2{ transf.pos.x, transf.pos.y };
    }
}

void CameraSystem::update()
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        if (state == FOLLOW_PLAYER)
        {
            // Camera target follows player
            camera.target = Vector2{ transf.pos.x, transf.pos.y  };

            if (IsKeyPressed(KEY_A))
            {
                state = FREEROAM;
                camera.target.x -= 2;
            }
            if (IsKeyPressed(KEY_D))
            {
                state = FREEROAM;
                camera.target.x += 2;
            }
            if (IsKeyPressed(KEY_W))
            {
                state = FREEROAM;
                camera.target.y -= 2;
            }
            if (IsKeyPressed(KEY_S))
            {
                state = FREEROAM;
                camera.target.y += 2;
            }

        }

        else
        {
            if (IsKeyDown(KEY_A))
            {
                camera.target.x -= 2;
            }
            if (IsKeyDown(KEY_D))
            {
                camera.target.x += 2;
            }
            if (IsKeyDown(KEY_W))
            {
                camera.target.y += 2;
            }
            if (IsKeyDown(KEY_S))
            {
                camera.target.y -= 2;
            }
        }

        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40;

        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;

            state = FOLLOW_PLAYER;
        }
    }
}

void CameraSystem::BeginCameraMode()
{
    BeginMode2D(camera);
}

void CameraSystem::EndCameraMode()
{
    EndMode2D();
}