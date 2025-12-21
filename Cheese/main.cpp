#pragma once 
#include "raylib.h"

#include "Coordinator.hpp"
#include "RenderSystem.h"
#include "PlayerMovementSystem.h"
#include "LevelManager.h"


Coordinator gCoordinator;

int main(void)
{
    LevelManager mgnr;
    InitWindow(900, 600, "THE TUTORING SHOW");
    SetTargetFPS(60);

    // wow this is bad, make a serializer / deserializer to load this shit in the init or something nat
    gCoordinator.init();

    mgnr.register_components();

    auto movement_system = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();


    mgnr.set_signatures();
    mgnr.place_objs();

    render_sys.get()->init();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE CALLS
        movement_system->update(deltaTime);

        // DRAW CALLS
        
        ClearBackground(WHITE);
        // temp tb  h 
        BeginDrawing();

        render_sys->draw();

        EndDrawing();
    }
}