#include "Coordinator.hpp"
#include "raylib.h"

// systems
#include "RenderSystem.h"
#include "PlayerMovementSystem.h"

// components
#include "Core.h"
#include "Player.h"

Coordinator gCoordinator;

void register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<render>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();

    gCoordinator.RegisterComponent<player>();
}

void set_system_signatures()
{
    // ALL of them should have status...
    Signature sig;
    sig.set(gCoordinator.GetComponentType<status>());
    sig.set(gCoordinator.GetComponentType<render>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<RenderSystem>(sig);

    sig.reset();
    sig.set(gCoordinator.GetComponentType<status>());

    sig.set(gCoordinator.GetComponentType<player>());
    sig.set(gCoordinator.GetComponentType<transform2D>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

}


void place_temp_testing_objs()
{
    //TEMPORARY DONT DO THIS
    Texture2D temp = LoadTexture("art/0.png");
    // player
    int ec = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            ec,
            render{ Vector2{100, 100}, WHITE, temp });
        gCoordinator.AddComponent(
            ec,
            transform2D{ Vector2 {200.0f, 300.0f} });
        gCoordinator.AddComponent(
            ec,
            player{true, 100.0f, 100.0f });
        gCoordinator.AddComponent(
            ec,
            collidble{ Rectangle{200, 400, 100, 100 } });
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
            render{ Vector2{w, 100}, BLACK });
        gCoordinator.AddComponent(
            ec,
            transform2D{ Vector2 {0, y  - 100} });
        gCoordinator.AddComponent(
            ec,
            collidble{ Rectangle{0, y - 100, w, 100 } });
        gCoordinator.AddComponent(
            ec,
            status{ true, true, WALL });
    }
}

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "TBD");
    SetTargetFPS(60);

    InitAudioDevice();

    gCoordinator.init();

    register_components();

    /*** Systems Creation ******************************************************************/
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto player_movement_sys = gCoordinator.RegisterSystem<PlayerMovementSystem>();

    set_system_signatures();

    place_temp_testing_objs();

    render_sys->init();
    player_movement_sys->init();

    // IN-GAME
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE
        {
            player_movement_sys->update(deltaTime);
        }


        // DRAW
        {
            ClearBackground(SKYBLUE);
            BeginDrawing();

            render_sys->draw();

            EndDrawing();
        }
    }

    // DE-INITIALIZATION
}