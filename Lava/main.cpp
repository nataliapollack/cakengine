#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <algorithm> 

#include "Tooling.h"

// temp while we still place objs here
// components
#include "Core.h"
#include "Player.h"
#include "Viewport3D.hpp"

// systems
#include "RenderingSystem.h"
#include "PlayerMovement.h"
#include "CollisionSystem.h"

// managers
#include "EnergyManager.h"
#include "ScreenManager.h"
#include "CycleManager.h"

// minigames
#include "SolarPanels.h"

void temp_place_objs();

Coordinator gCoordinator;
Model gTeapotModel_temp; // TODO: REMOVE
Camera2D gCamera;

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "AGNUS");
    SetTargetFPS(60);

    gCoordinator.init();

    Tooling tools;

    tools.register_components();

    // systems
    auto player_movement_sys = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto collision_sys = gCoordinator.RegisterSystem<CollisionSystem>();

    // managerss
    EnergyManager enery_mngr;
    ScreenManager screen_mngr;
    CycleManager cycle_mngr;

    // minigames
    SolarPanel solar_game;

    tools.set_system_signatures();

    temp_place_objs();

    // inits
    render_sys->init();
    player_movement_sys->init();

    enery_mngr.init();
    screen_mngr.init();
    cycle_mngr.init();

    solar_game.init();

    // camera Things
    gCamera.target = Vector2{ 0, 0 };
    gCamera.zoom = 1.0f;


    //**************  HEY! SET YOUR SCREEN HERE IF NEEDED ****************
    screen_mngr.SetScreen(INSIDE);

    while (!WindowShouldClose())
    {
        // update
        float deltaTime = 1.0f / 60.0f;

        cycle_mngr.UpdateTimer(deltaTime);
        if (screen_mngr.GetScreen() == OUTSIDE || screen_mngr.GetScreen() == INSIDE)
        {
            player_movement_sys->move_player(deltaTime);
            collision_sys->CheckCollisions();
        }

        //draw
        {
            // must be done before "BeginDrawing"
            render_sys->render_viewports();

            BeginDrawing();

            // things that are 2D
            BeginMode2D(gCamera);
            {
                if (screen_mngr.GetScreen() == INSIDE)
                {
                    ClearBackground(GRAY);
                }
                if (screen_mngr.GetScreen() == OUTSIDE)
                {
                    ClearBackground(DARKGRAY);

                    render_sys->draw();

                    enery_mngr.DrawEnergyLevels();

                    cycle_mngr.draw();

                }
            }
            EndMode2D();

            if (screen_mngr.GetScreen() == SOLAR_TILE)
            {
                ClearBackground(DARKGRAY);
                solar_game.draw();

            }
            if (screen_mngr.GetScreen() == MAZE)
            {
                ClearBackground(DARKBLUE);
            }
            if (screen_mngr.GetScreen() == AUDIO_FREQ)
            {
                ClearBackground(RED);
            }
            if (screen_mngr.GetScreen() == CAMERAS)
            {
                ClearBackground(DARKGREEN);
            }

            EndDrawing();
        }
    }

    UnloadModel(gTeapotModel_temp);
    render_sys->shutdown();

    CloseWindow();
}

// until tools gets made
void temp_place_objs()
{
    Entity en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{100.0f, 100.0f },  WHITE }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {200.0f, 200.0f } }
    );

    gCoordinator.AddComponent(
        en,
        status{true, true, OUTSIDE, PLAYER }
    );

    gCoordinator.AddComponent(
        en,
        player{ Vector2{30.0f, 30.0f} }
    );

    // 3D ENTITY BILLBOARD TEST
    Entity teapot = gCoordinator.CreateEntity();
    gTeapotModel_temp = LoadModel("art/teapot.obj");
    // TODO: TEMP! REPLACE WITH ASSET REF WHEN ASSET MANAGER IMPLEMENTED
    gCoordinator.AddComponent(
        teapot,
        model_view{ .model = &gTeapotModel_temp }
    );
    gCoordinator.AddComponent(
        teapot,
        viewport3D{
            .view = {
                .position = { 0, 1, 5 },
                .target = { 0 },
                .up = { 0, 1, 0 },
                .fovy = { 10 },
                .projection = { CAMERA_ORTHOGRAPHIC }
            },
            .framebuffer = LoadRenderTexture(200, 200)
        }
    );
    gCoordinator.AddComponent(
        teapot,
        transform2D{ { 450, 150 } }
    );
    gCoordinator.AddComponent(
        teapot,
        render_box{ { 200, 200 }, WHITE }
    );
}