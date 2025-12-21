
#include "PlayerMovementSystem.h"
#include "Coordinator.hpp"
#include "raylib.h"
#include "Game.h"

#include "components/Transform.h"
#include "components/Player.h"

extern Coordinator gCoordinator;

void PlayerMovementSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::CHANGEROOM, PlayerMovementSystem::ChangeBounds));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, PlayerMovementSystem::ChangeDay));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::DOOROPEN, PlayerMovementSystem::ChangeLock));

    camera = { 0 };
    //camera.target = Vector2{ player.x + 20.0f, player.y + 20.0f };
    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.6f;

    cam_bounds = Vector2{ 410.0f, 560.0f };
    player_bounds = Vector2{ 162.0f, 680.0f };
    //cam_bounds = Vector2{ -5000.0f, 5000.0f };
    //player_bounds = Vector2{ -5000.0f, 5000.0f };

    //float updated_x = (float)GetScreenWidth();
    ////        cam_bounds.y = 940.0f + updated_x;
    //player_bounds.y = 1027.0f + updated_x;
    //player_bounds.x += updated_x;

    //cam_bounds.y = 940.0f + updated_x;
    //cam_bounds.x += updated_x;

    day = 0;

    room = 1;
    locked = true;
}

void PlayerMovementSystem::update(float dt)
{
    // there should only ever be one in here lol
    for (auto& entity : entities_list)
    {
        auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
        auto& playuh = gCoordinator.GetComponent<Player>(entity);

        if (IsKeyDown(KEY_LEFT))
        {
            transform.position.x -= (playuh.speed * dt);

            if (transform.position.x < player_bounds.x)
                transform.position.x = player_bounds.x;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            transform.position.x += (playuh.speed * dt);

            if (transform.position.x > player_bounds.y)
                transform.position.x = player_bounds.y;
        }

        if (!locked)
        {
            if (IsKeyDown(KEY_DOWN))
            {
                transform.position.y += (playuh.speed * dt);

                if (transform.position.y > 562)
                    transform.position.y = 562;
            }
            if (IsKeyDown(KEY_UP))
            {
                transform.position.y -= (playuh.speed * dt);
                if (transform.position.y < 348)
                    transform.position.y = 348;
            }
        }
        Vector2 cam = Vector2{ transform.position.x + 20, transform.position.y + 20 };

        if (cam.x < cam_bounds.x)
        {
            cam.x = cam_bounds.x;
        }
        if (cam.x > cam_bounds.y)
        {
            cam.x = cam_bounds.y;
        }
        // Camera target follows player
        camera.target = cam;


        // Camera zoom controls
        // Uses log scaling to provide consistent zoom speed
        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }
    }
}

void PlayerMovementSystem::BeginDrawCam()
{
    BeginMode2D(camera);
}

void PlayerMovementSystem::EndDrawCam()
{
    EndMode2D();
}

void PlayerMovementSystem::ChangeBounds(Event& event)
{
     //bedroom == 0
     //hallways == 1 lol

    switch (room)
    {
    case 1:
    {
        cam_bounds.y = 940.0f;
        player_bounds.y = 1027.0f;

        room = 0;
        break;
    }
    case 0:
    {
        cam_bounds.y = 560;
        player_bounds.y = 680;
        room = 1;
        break;
    }
    }

    if (day == 4)
    {
        float updated_x = (float)GetScreenWidth();
        //        cam_bounds.y = 940.0f + updated_x;
        player_bounds.y = 1027.0f + updated_x;
        player_bounds.x += updated_x;

        cam_bounds.y = 940.0f + updated_x;
        cam_bounds.x += updated_x;
        
    }
}

void PlayerMovementSystem::ChangeDay(Event& event)
{
    day += 1;
}

void PlayerMovementSystem::ChangeLock(Event& event)
{
    locked = false;

    cam_bounds.x = -5000;
    cam_bounds.y = 5000;

    player_bounds.x = 94;
    player_bounds.y = 1978;
}