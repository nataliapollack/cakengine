#include "Coordinator.hpp"
#include "raylib.h"

// systems
#include "RenderSystem.h"

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

}


void place_temp_testing_objs()
{
    // player
    int ec = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            ec,
            render{ Vector2{100, 100}, WHITE });
        gCoordinator.AddComponent(
            ec,
            transform2D{ Vector2 {200.0f, 400.0f} });
        gCoordinator.AddComponent(
            ec,
            player{ });
        gCoordinator.AddComponent(
            ec,
            collidble{ Rectangle{200, 400, 100, 100 } });
        gCoordinator.AddComponent(
            ec,
            status{ true, true });
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


    set_system_signatures();

    place_temp_testing_objs();
    render_sys->init();

    // IN-GAME
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE

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