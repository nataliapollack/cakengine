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
#include "AssetManager.hpp"

// minigames
#include "SolarPanels.h"
#include "NightMinigame.h"
#include "MazeBuilder.h"


Coordinator gCoordinator;
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
    NightMinigame night_game;
    MazeBuilder maze_game;

    tools.set_system_signatures();

    AssetManager::get().load_all("art");
    tools.place_objs();

    // inits
    render_sys->init();
    player_movement_sys->init();

    enery_mngr.init();
    screen_mngr.init();
    cycle_mngr.init();

    solar_game.init();
    night_game.init();
    maze_game.Init();

    // camera Things
    gCamera.target = Vector2{ 0, 0 };
    gCamera.zoom = 1.0f;


    //**************  HEY! SET YOUR SCREEN HERE IF NEEDED ****************
    screen_mngr.SetScreen(CAMERAS);
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
        else if (screen_mngr.GetScreen() == CAMERAS) {
            night_game.update(deltaTime);
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

                if (maze_game.mazeBuilt)
                {
                    maze_game.UpdateMaze(deltaTime);
                }
                maze_game.DrawMaze();

            }
            if (screen_mngr.GetScreen() == AUDIO_FREQ)
            {
                ClearBackground(RED);
            }
            if (screen_mngr.GetScreen() == CAMERAS)
            {
                ClearBackground(DARKGREEN);
                night_game.draw();
            }

            if (screen_mngr.GetIsFading())
            {
                screen_mngr.UpdateFadeTransition(deltaTime);
            }
            EndDrawing();
        }
    }

    render_sys->shutdown();
    AssetManager::get().unload_all();

    CloseWindow();
}