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
    state = FREEROAM;
    transition = false;
    obtained_entity = false;
    current_entity = 0;

    timer = 2.0f;
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::UPDATECAM,
            CameraSystem::SetCurrentEn));
}


//function easeOutQuad(x: number) : number{
//return 1 - (1 - x) * (1 - x);
//}

float lerp(float t, float old_x, float new_x)
{
    return old_x * (1 - t) + (new_x * t);
}

float easeOut(float x)
{
    return (1 - (1 - x) * (1 - x));
}

//{
//    return (1 - (1 - x) * (1 - x));
//}

void CameraSystem::update(float dt)
{

    if (IsKeyPressed(KEY_R))
    {
     state = FREEROAM;
        
    }
    if (IsKeyPressed(KEY_P))
    {
        state = FOLLOW_PLAYER;
    }

    gCamera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    if (state == FOLLOW_PLAYER)
    {
        if (obtained_entity)
        {
            float t = easeOut(1.0f - (timer / 2.0f));

            gCamera.target = Vector2
            {
                lerp(t, old_point.x, new_point.x),
                lerp(t, old_point.y, new_point.y)
            };

            gCamera.zoom = lerp(t, old_zoom, new_zoom);

            timer -= dt;
            if (timer <= 0)
            {
                timer = 2.0f;
                obtained_entity = false;
            }
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
    Entity ec = event.GetParam<Entity>(Events::Collision::UPDATECAM_ID);

    if (ec != current_entity)
    {
        transition = true;
        obtained_entity = true;
        current_entity = ec;
        old_point = gCamera.target;
        old_zoom = gCamera.zoom;

        auto& transf = gCoordinator.GetComponent<camera_info>(current_entity);
        //Camera target follows player
        new_point = Vector2{ transf.x, transf.y };
        new_zoom = transf.zoom;
    }
}