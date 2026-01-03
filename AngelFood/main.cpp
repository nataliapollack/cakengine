#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <algorithm> 

// systems
#include "RenderSystem.h"
#include "PlayerSystem.h"
#include "CameraSystem.h"
#include "ItemSystem.h"
#include "CollisionSystem.h"
#include "Tooling.h"
#include "ParticleSystem.h"

// components
#include "Core.h"
#include "Player.h"
#include "Progression.h"
#include "ParticleEmitter.h"

// msc
#include "AssetManager.h"

Coordinator gCoordinator;
AssetManager gAssetMngr;

//TEMP SORRY
Camera2D gCamera;

void register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<render>();
    gCoordinator.RegisterComponent<box_render>();
    gCoordinator.RegisterComponent<render_environment>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();

    gCoordinator.RegisterComponent<player>();

    gCoordinator.RegisterComponent<collectable>();
    gCoordinator.RegisterComponent<collecting>();

    gCoordinator.RegisterComponent<physics>();

    gCoordinator.RegisterComponent<particle_emitter>();
}

void set_system_signatures()
{
    Signature sig;
    sig.set(gCoordinator.GetComponentType<render>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<RenderSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<box_render>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<BoxRenderSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<render_environment>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<EnvironmentRenderSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<player>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<PlayerSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<player>());
    gCoordinator.SetSystemSignature<CameraSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<collectable>());
    gCoordinator.SetSystemSignature<ItemSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<collidble>());
    gCoordinator.SetSystemSignature<CollisionSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<collecting>());
    gCoordinator.SetSystemSignature<CollectingSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<status>());
    gCoordinator.SetSystemSignature<Tooling>(sig);

    sig.reset();
    
    sig.set(gCoordinator.GetComponentType<particle_emitter>());
    gCoordinator.SetSystemSignature<ParticleSystem>(sig);
}

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "ICA");
    SetTargetFPS(60);

    InitAudioDevice();

    gCoordinator.init();
    gAssetMngr.load_assets();

    register_components();

    /*** Systems Creation ******************************************************************/
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto player_movement_sys = gCoordinator.RegisterSystem<PlayerSystem>();
    auto camera_sys = gCoordinator.RegisterSystem<CameraSystem>();
    auto item_sys = gCoordinator.RegisterSystem<ItemSystem>();
    auto collision_sys = gCoordinator.RegisterSystem<CollisionSystem>();
    auto box_render_sys = gCoordinator.RegisterSystem<BoxRenderSystem>();
    auto dropoff_sys = gCoordinator.RegisterSystem<CollectingSystem>();
    auto environment_render_sys = gCoordinator.RegisterSystem<EnvironmentRenderSystem>();
    auto tooling_sys = gCoordinator.RegisterSystem<Tooling>();
    auto particle_sys = gCoordinator.RegisterSystem<ParticleSystem>();


    tooling_sys->init();

    set_system_signatures();

    tooling_sys->deserialize();

    ////// object placer heree until i rlly like add some way to do player state mngr...
    //  {
           //player
          //int ec = gCoordinator.CreateEntity();
          //{
          //    gCoordinator.AddComponent(
          //        ec,
          //        render_environment{ 0, PLAYER_IDLE, 1});
          //    gCoordinator.AddComponent(
          //        ec,
          //        transform2D{ Vector2 {-50.0f, -50.0f} });
          //    gCoordinator.AddComponent(
          //        ec,
          //        player{ true, NONE });
          //    gCoordinator.AddComponent(
          //        ec,
          //        collidble{ Rectangle{00, 00, 100, 100 } });
          //    gCoordinator.AddComponent(
          //        ec,
          //        status{ true, true, PLAYER });
          //    gCoordinator.AddComponent(
          //        ec,
          //        physics{ Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f} }
          //    );
          //}

          // //floor...
          //int ec = gCoordinator.CreateEntity();
          //{
          //    float w = GetScreenWidth();
          //    float y = GetScreenHeight();
          //    gCoordinator.AddComponent(
          //        ec,
          //        box_render{ w, 100 });
          //    gCoordinator.AddComponent(
          //        ec,
          //        transform2D{ Vector2 {0, 500} });
          //    gCoordinator.AddComponent(
          //        ec,
          //        collidble{ Rectangle{0, y - 100, w, 100 } });
          //    gCoordinator.AddComponent(
          //        ec,
          //        status{ true, true, WALL });
          //}

    //  tooling_sys->serialize();
    //  }

//    tooling_sys->serialize();

    render_sys->init();
    box_render_sys->init();
    player_movement_sys->init();
    camera_sys->init();
    item_sys->init();
    dropoff_sys->init();
    environment_render_sys->init();
    particle_sys->init();

    // IN-GAME
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        //float clamped_dt = std::clamp(deltaTime, 0.01f, 0.03f);
        float clamped_dt = std::clamp(deltaTime, 1.0f / 60.0f, 1.0f / 60.0f);

        // UPDATE
        {
            if (!tooling_sys->GetToolStatus())
            {
                player_movement_sys->update(clamped_dt);
                collision_sys->CheckCollisions();
                particle_sys->update(clamped_dt);
            }

            camera_sys->update();
            tooling_sys->update();

            // hard coded resett
            if (IsKeyPressed(KEY_TWO))
            {
                player_movement_sys->ResetPlayerPos();
            }
        }

        // DRAW
        {
            BeginDrawing();

            if (tooling_sys->GetToolStatus())
            {
                ClearBackground(DARKGRAY);
            }
            else
            {
                ClearBackground(DARKBLUE);
            }

            camera_sys->BeginCameraMode();


            rlPushMatrix();
                rlTranslatef(0, 25 * 50, 0);
                rlRotatef(90, 1, 0, 0);
                DrawGrid(500, 100);
            rlPopMatrix();

            

            environment_render_sys->draw();
            render_sys->draw();
            box_render_sys->draw();

            particle_sys->draw();

            if (tooling_sys->GetToolStatus())
            {
                tooling_sys->draw();
            }

           // collision_sys->debug_draw_collisions();

         //   DrawCircle(0, 0, 10, BLUE);
            camera_sys->EndCameraMode();


            DrawText(TextFormat("dt %f", clamped_dt), 20, 20, 40, RED);
            
            // Particles

            EndDrawing();
        }
    }

    // DE-INITIALIZATION

    gAssetMngr.unload();

    CloseAudioDevice();
    CloseWindow();
}