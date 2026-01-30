#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <algorithm> 

// components
#include "Core.h"
#include "Player.h"
#include "Viewport3D.hpp"

// systems
#include "RenderingSystem.h"
#include "PlayerMovement.h"

// managers
#include "EnergyManager.h"
#include "ScreenManager.h"

void temp_place_objs();
void register_components();
void set_system_signatures();

Coordinator gCoordinator;
Model gTeapotModel_temp; // TODO: REMOVE

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "AGNUS");
    SetTargetFPS(60);

    gCoordinator.init();

    register_components();

    // systems
    auto player_movement_sys = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();


    // managerss
    EnergyManager enery_mngr;
    ScreenManager screen_mngr;

    set_system_signatures();

    temp_place_objs();

    // inits
    render_sys->init();

    enery_mngr.init();
    screen_mngr.init();


    //**************  HEY! SET YOUR SCREEN HERE IF NEEDED ****************
    screen_mngr.SetScreen(GAME);

    while (!WindowShouldClose())
    {
        // update
        float deltaTime = 1.0f / 60.0f;

        if (screen_mngr.GetScreen() == GAME)
        {
            player_movement_sys->move_player(deltaTime);
        }

        //draw
        {
            // must be done before "BeginDrawing"
            render_sys->render_viewports();

            BeginDrawing();

            if (screen_mngr.GetScreen() == GAME)
            {                
                ClearBackground(BLACK);

                render_sys->draw();

                enery_mngr.DrawEnergyLevels();

            }
            if (screen_mngr.GetScreen() == MAZE)
            {
                ClearBackground(BLUE);
            }
            if (screen_mngr.GetScreen() == AUDIO_FREQ)
            {
                ClearBackground(RED);
            }
            if (screen_mngr.GetScreen() == SOLAR_TILE)
            {
                ClearBackground(GREEN);
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
        status{true, PLAYER }
    );

    gCoordinator.AddComponent(
        en,
        player{20.0f}
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

void register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<player>();
    gCoordinator.RegisterComponent<render_box>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();

    gCoordinator.RegisterComponent<viewport3D>();
    // TODO: TEMP! REPLACE WITH ASSET REF WHEN ASSET MANAGER IMPLEMENTED
    gCoordinator.RegisterComponent<model_view>();
}

void set_system_signatures()
{
    Signature sig;
    sig.set(gCoordinator.GetComponentType<player>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

    sig.reset();
}