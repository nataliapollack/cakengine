#include "Coordinator.hpp"
#include "raylib.h"

// systems
#include "RenderSystem.h"
#include "PlayerSystem.h"
#include "CameraSystem.h"
#include "ItemSystem.h"
#include "CollisionSystem.h"


// components
#include "Core.h"
#include "Player.h"
#include "Progression.h"

// msc
#include "AssetManager.h"

Coordinator gCoordinator;
AssetManager gAssetMngr;

void register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<render>();
    gCoordinator.RegisterComponent<box_render>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();

    gCoordinator.RegisterComponent<player>();

    gCoordinator.RegisterComponent<collectable>();
    gCoordinator.RegisterComponent<collecting>();

    gCoordinator.RegisterComponent<physics>();
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

    sig.set(gCoordinator.GetComponentType<player>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<PlayerSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<player>());
    gCoordinator.SetSystemSignature<CameraSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<collectable>());
 //   sig.set(gCoordinator.GetComponentType<collecting>());
    gCoordinator.SetSystemSignature<ItemSystem>(sig);

    sig.reset();
    sig.set(gCoordinator.GetComponentType<collidble>());
    gCoordinator.SetSystemSignature<CollisionSystem>(sig);

}


int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "TBD");
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


    set_system_signatures();

    // object placer heree until i rlly like add some way to do player state mngr...
    {
        // player
        int ec = gCoordinator.CreateEntity();
        {
            gCoordinator.AddComponent(
                ec,
                render{ 0.5f, PLAYER_IDLE});
            gCoordinator.AddComponent(
                ec,
                transform2D{ Vector2 {200.0f, 100.0f} });
            gCoordinator.AddComponent(
                ec,
                player{ true, NONE });
            gCoordinator.AddComponent(
                ec,
                collidble{ Rectangle{200, 300, 100, 100 } });
            gCoordinator.AddComponent(
                ec,
                status{ true, true, PLAYER });
        }

        // floor...
        ec = gCoordinator.CreateEntity();
        {
            float w = GetScreenWidth();
            float y = GetScreenHeight();
            gCoordinator.AddComponent(
                ec,
                box_render{ w, 100, BLACK });
            gCoordinator.AddComponent(
                ec,
                transform2D{ Vector2 {0, 500} });
            gCoordinator.AddComponent(
                ec,
                collidble{ Rectangle{0, y - 100, w, 100 } });
            gCoordinator.AddComponent(
                ec,
                status{ true, true, WALL });
        }

        // item
        ec = gCoordinator.CreateEntity();
        {
            gCoordinator.AddComponent(
                ec,
                render{ 1.0f, TEMP_ITEM});
            gCoordinator.AddComponent(
                ec,
                transform2D{ Vector2 {500.0f, 300.0f} });
            gCoordinator.AddComponent(
                ec,
                collectable{ false });
            gCoordinator.AddComponent(
                ec,
                collidble{Rectangle{200, 400, 100, 100 } });
            gCoordinator.AddComponent(
                ec,
                status{ true, true, ITEM });
        }
    }

    render_sys->init();
    box_render_sys->init();
    player_movement_sys->init();
    camera_sys->init();
    item_sys->init();

    // IN-GAME
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE
        {
            player_movement_sys->update(deltaTime);
            collision_sys->CheckCollisions();
            camera_sys->update();
        }


        // DRAW
        {
            BeginDrawing();

            camera_sys->BeginCameraMode();
            ClearBackground(SKYBLUE);
            
            box_render_sys->draw();

            render_sys->draw();


            //collision_sys->debug_draw_collisions();

            camera_sys->EndCameraMode();

            EndDrawing();
        }
    }

    // DE-INITIALIZATION

    gAssetMngr.unload();

    CloseAudioDevice();
    CloseWindow();
}