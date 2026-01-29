#pragma once 
#include "raylib.h"

#include "Coordinator.hpp"
#include "RenderSystem.h"
#include "PlayerSystem.h"
#include "LevelManager.h"


Coordinator gCoordinator;

int main(void)
{
    LevelManager mgnr;
    InitWindow(900, 600, "The Graveyard Shift");
    SetTargetFPS(60);

    gCoordinator.init();

    mgnr.register_components();


    auto player_system = gCoordinator.RegisterSystem<PlayerSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();


    mgnr.set_signatures();
    mgnr.place_objs();

    render_sys.get()->init();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE CALLS
        player_system->update(deltaTime);

        // DRAW CALLS
        
        ClearBackground(BLACK);

        BeginDrawing();

        render_sys->draw();

        EndDrawing();
    }

   // CloseAudioDevice();
    CloseWindow();
}