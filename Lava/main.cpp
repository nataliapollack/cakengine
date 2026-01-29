#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <algorithm> 

// components
#include "Core.h"
#include "Player.h"

// systems
#include "RenderingSystem.h"
#include "PlayerMovement.h"


void temp_place_objs();
void register_components();
void set_system_signatures();

Coordinator gCoordinator;

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "AGNUS");
    SetTargetFPS(60);

    gCoordinator.init();

    register_components();

    auto player_movement_sys = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();

    set_system_signatures();

    temp_place_objs();

    render_sys->init();

    while (!WindowShouldClose())
    {
        // update
        float deltaTime = 1.0f / 60.0f;

        player_movement_sys->move_player(deltaTime);

        //draw
        {
            BeginDrawing();

            ClearBackground(RED);

            render_sys->draw();

            EndDrawing();
        }
    }

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
}

void register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<player>();
    gCoordinator.RegisterComponent<render_box>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();
}

void set_system_signatures()
{
    Signature sig;
    sig.set(gCoordinator.GetComponentType<player>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

    sig.reset();
}