#include "CameraSystem.h"

#include "Coordinator.hpp"
#include "Core.h"

extern Coordinator gCoordinator;
extern Camera2D gCamera;

void CameraSystem::init()
{
    gCamera = { 0 };
    gCamera.rotation = 0.0f;
    gCamera.zoom = 0.60;
    state = FREEROAM;

    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        gCamera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        gCamera.target = Vector2{ transf.pos.x, transf.pos.y };
    }
}

void CameraSystem::update()
{
    // there should only ever be one in here lol
   for (auto& entity : entities_list)

    {
        auto& transf = gCoordinator.GetComponent<transform2D>(entity);
        gCamera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        if (state == FOLLOW_PLAYER)
        {
           //  Camera target follows player
            gCamera.target = Vector2{ transf.pos.x, transf.pos.y  };

            if (IsKeyPressed(KEY_A))
            {
                state = FREEROAM;
                gCamera.target.x -= 5;
            }
            if (IsKeyPressed(KEY_D))
            {
                state = FREEROAM;
                gCamera.target.x += 5;
            }
            if (IsKeyPressed(KEY_W))
            {
                state = FREEROAM;
                gCamera.target.y -= 5;
            }
            if (IsKeyPressed(KEY_S))
            {
                state = FREEROAM;
                gCamera.target.y += 5;
            }

        }

        else
        {
            if (IsKeyDown(KEY_A))
            {
                gCamera.target.x -= 5;
            }
            if (IsKeyDown(KEY_D))
            {
                gCamera.target.x += 5;
            }
            if (IsKeyDown(KEY_W))
            {
                gCamera.target.y -= 5;
            }
            if (IsKeyDown(KEY_S))
            {
                gCamera.target.y += 5;
            }
        }

        if (gCamera.rotation > 40) gCamera.rotation = 40;
        else if (gCamera.rotation < -40) gCamera.rotation = -40;

        gCamera.zoom = expf(logf(gCamera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

        if (IsKeyPressed(KEY_R))
        {
            gCamera.zoom = 0.60f;
            gCamera.rotation = 0.0f;

            state = FOLLOW_PLAYER;
        }
    }
}

void CameraSystem::BeginCameraMode()
{
    BeginMode2D(gCamera);
}

void CameraSystem::EndCameraMode()
{
    EndMode2D();
}