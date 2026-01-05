#include "CameraSystem.h"

#include "Coordinator.hpp"
#include "Core.h"
#include "Progression.h"

extern Coordinator gCoordinator;
extern Camera2D gCamera;

void CameraSystem::init()
{
    gCamera = { 0 };
    gCamera.rotation = 0.0f;
    gCamera.zoom = 0.60;
    state = FOLLOW_PLAYER;

    obtained_entity = false;
    
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::UPDATECAM,
            CameraSystem::SetCurrentEn));
}

void CameraSystem::update()
{

    gCamera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    if (state == FOLLOW_PLAYER)
    {
        if (obtained_entity)
        {
            auto& transf = gCoordinator.GetComponent<camera_info>(current_entity);
            //Camera target follows player
            gCamera.target = Vector2{ transf.x, transf.y };
            gCamera.zoom = transf.zoom;
            obtained_entity = false;
        }

        if (IsKeyPressed(KEY_A))
        {
            state = FREEROAM;
            gCamera.target.x -= 10;
        }
        if (IsKeyPressed(KEY_D))
        {
            state = FREEROAM;
            gCamera.target.x += 10;
        }
        if (IsKeyPressed(KEY_W))
        {
            state = FREEROAM;
            gCamera.target.y -= 10;
        }
        if (IsKeyPressed(KEY_S))
        {
            state = FREEROAM;
            gCamera.target.y += 10;
        }

    }

    else
    {
        if (IsKeyDown(KEY_A))
        {
            gCamera.target.x -= 50;
        }
        if (IsKeyDown(KEY_D))
        {
            gCamera.target.x += 50;
        }
        if (IsKeyDown(KEY_W))
        {
            gCamera.target.y -= 50;
        }
        if (IsKeyDown(KEY_S))
        {
            gCamera.target.y += 50;
        }
    }

    if (gCamera.rotation > 40) gCamera.rotation = 40;
    else if (gCamera.rotation < -40) gCamera.rotation = -40;

    gCamera.zoom = expf(logf(gCamera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

    if (IsKeyPressed(KEY_I))
    {
        Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);
        gCamera.target = mouse_pos;
        state = FOLLOW_PLAYER;
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

void CameraSystem::SetCurrentEn(Event& event)
{
    obtained_entity = true;
    current_entity = event.GetParam<Entity>(Events::Collision::UPDATECAM);
}