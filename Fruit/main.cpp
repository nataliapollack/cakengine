#include "raylib.h"
#include "Coordinator.hpp"

#include "Events.hpp"
#include "Game.h"

#include "ObjPlacer.h"
#include "TimerManager.h"
#include "Menu.h"

// systems
#include "CameraSystem.h"
#include "PlayerMovementSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "ObstacleSystem.h"

// components
#include "Core.h"
#include "Player.h"
#include "Obstacles.h"

Coordinator gCoordinator;

void register_components()
{
    gCoordinator.RegisterComponent<transform2>();
    gCoordinator.RegisterComponent<player>();
    gCoordinator.RegisterComponent<render_box>();
    gCoordinator.RegisterComponent<collidible>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<obstacle>();
}

void set_signatures()
{
    Signature sig;

    sig.set(gCoordinator.GetComponentType<player>());
    sig.set(gCoordinator.GetComponentType<transform2>());
    gCoordinator.SetSystemSignature<PlayerMovement>(sig);

    sig.reset();
    
    sig.set(gCoordinator.GetComponentType<render_box>());
    sig.set(gCoordinator.GetComponentType<transform2>());
    sig.set(gCoordinator.GetComponentType<status>());
    gCoordinator.SetSystemSignature<RenderSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<transform2>());
    sig.set(gCoordinator.GetComponentType<collidible>());
    gCoordinator.SetSystemSignature<CollisionSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<transform2>());
    sig.set(gCoordinator.GetComponentType<obstacle>());
    gCoordinator.SetSystemSignature<ObstacleSystem>(sig);

}


int main()
{
    InitWindow(900, 600, "Bandit's Ski Trip!");
    SetTargetFPS(60);

    gCoordinator.init();

    // INITIALIZATION
    register_components();

    ObjPlacer obj_placer;
    SkiTimer ski_timer;
    Menus menu;
    
        // register systems
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto movement_sys = gCoordinator.RegisterSystem<PlayerMovement>();
    auto collision_sys = gCoordinator.RegisterSystem<CollisionSystem>();
    auto obstacle_sys = gCoordinator.RegisterSystem<ObstacleSystem>();

    render_sys->init();
    movement_sys->init();
    obstacle_sys->init();

    set_signatures();

    ski_timer.init();
    obj_placer.init();
    menu.init();
    
    obj_placer.place_player();
    obj_placer.place_ski_objs();

    bool ski_ongoing = false;
    bool ski_finished = false;

    // IN-GAME
    while (!menu.get_status())
    {
        float deltaTime = GetFrameTime();

        // UPDATE
        if (!menu.get_menu())
        {
            movement_sys->update(deltaTime);
            obstacle_sys->update(deltaTime);
            collision_sys->CheckCollisions();

            ski_timer.update();
        }
        else
        {
            menu.update();
            menu.set_scores(ski_timer.GetPoints());
        }

        if (menu.get_reset())
        {
            menu.reset();
            ski_timer.reset_timer();
            obstacle_sys->reset();
        }

        // DRAW

        ClearBackground(SKYBLUE);
        BeginDrawing();

        render_sys->draw();
       // obstacle_sys->display_obj_count_debug();
        ski_timer.draw();

        if (menu.get_menu())
            menu.draw();


        EndDrawing();
    }

    // DE-INITIALIZATION

    CloseWindow();
}