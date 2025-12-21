#include "Coordinator.hpp"
#include "raylib.h"

#include "Game.h"

#include "ObjPlacer.h"

// components
#include "Core.h"
#include "Player.h"
#include "Obstacles.h"

extern Coordinator gCoordinator;


void ObjPlacer::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Race::FINISHED, ObjPlacer::place_finish_line));

    dog = LoadTexture("art/bandit.png");
    tree = LoadTexture("art/tree.png");
    flags = LoadTexture("art/flags.png");
}

void ObjPlacer::place_ski_objs()
{
    int ec;

    float w = 319 * 0.70;
    float h = 428 * 0.70;

    for (int i = 0; i < 6; i++)
    {
        int y = GetRandomValue(-100, -1000);
        int x = GetRandomValue(0, GetScreenWidth());

        ec = gCoordinator.CreateEntity();
        gCoordinator.AddComponent(
            ec,
            render_box{ w, h, BLUE, tree });
        gCoordinator.AddComponent(
            ec,
            transform2{ Vector2 {(float)x, (float)y} });
        gCoordinator.AddComponent(
            ec,
            obstacle{});
        gCoordinator.AddComponent(
            ec,
            collidible{ Vector2{ w, h}, OBJ_TYPES::OBSTACLE });
        gCoordinator.AddComponent(
            ec,
            status{ true, true });
    }

    int y = GetRandomValue(-100, -1000);
    int x = GetRandomValue(0, GetScreenWidth());
    ec = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        ec,
        render_box{ 250, 250, GREEN, flags});
    gCoordinator.AddComponent(
        ec,
        transform2{ Vector2 {(float)x, (float)y} });
    gCoordinator.AddComponent(
        ec,
        obstacle{});
    gCoordinator.AddComponent(
        ec,
        collidible{ Vector2{ 250, 250}, OBJ_TYPES::FLAG });
    gCoordinator.AddComponent(
        ec,
        status{ true, true });
}

void ObjPlacer::place_player()
{
    // player
    int ec = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            ec,
            render_box{ 128, 211, WHITE, dog });
        gCoordinator.AddComponent(
            ec,
            transform2{ Vector2 {200.0f, 400.0f} });
        gCoordinator.AddComponent(
            ec,
            player{ 100.0f, 30.0f });
        gCoordinator.AddComponent(
            ec,
            collidible{ Vector2{128, 211}, OBJ_TYPES::PLAYER });
        gCoordinator.AddComponent(
            ec,
            status{ true, true });
    }
}

void ObjPlacer::place_finish_line(Event &event)
{
    float x = GetScreenWidth();

    int ec = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        ec,
        render_box{ x, 200, RED, });
    gCoordinator.AddComponent(
        ec,
        transform2{ Vector2 {0, 0} });
    gCoordinator.AddComponent(
        ec,
        obstacle{});
    gCoordinator.AddComponent(
        ec,
        collidible{ Vector2{x, 200}, OBJ_TYPES::FINISH_LINE });
    gCoordinator.AddComponent(
        ec,
        status{ true, true });
}