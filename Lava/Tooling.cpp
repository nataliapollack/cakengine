#pragma once
#include "Tooling.h"

#include "Coordinator.hpp"

#include "Lava.h"
#include "raylib.h"

#include "AssetManager.hpp"
#include "ScreenManager.h"

// components
#include "Core.h"
#include "Player.h"
#include "Viewport3D.hpp"

// systems
#include "RenderingSystem.h"
#include "CollisionSystem.h"
#include "PlayerMovement.h"

extern Coordinator gCoordinator;

void Tooling::register_components()
{
    gCoordinator.RegisterComponent<transform2D>();
    gCoordinator.RegisterComponent<player>();
    gCoordinator.RegisterComponent<render_box>();
    gCoordinator.RegisterComponent<status>();
    gCoordinator.RegisterComponent<collidble>();

    gCoordinator.RegisterComponent<viewport3D>();
    // TODO: temp? replace with asset manager ref?
    gCoordinator.RegisterComponent<model_view>();
}

void Tooling::set_system_signatures()
{
    Signature sig;
    sig.set(gCoordinator.GetComponentType<player>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<collidble>());
    gCoordinator.SetSystemSignature<CollisionSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<render_box>());
    gCoordinator.SetSystemSignature<RenderSystem>(sig);

    sig.reset();
}

void Tooling::place_objs()
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
        status{ true, true, OUTSIDE, PLAYER }
    );

    gCoordinator.AddComponent(
        en,
        player{ Vector2{30.0f, 30.0f} }
    );

    // 3D ENTITY BILLBOARD TEST
    Entity teapot = gCoordinator.CreateEntity();

    auto& assetMngr = AssetManager::get();

    gCoordinator.AddComponent(
        teapot,
        model_view{ .model = &assetMngr.get<Model>("agnus") }
    );
    gCoordinator.AddComponent(
        teapot,
        viewport3D{
            .view = {
                .position = { 0, 1, 15 },
                .target = { 0 },
                .up = { 0, 1, 0 },
                .fovy = { 10 },
                .projection = { CAMERA_PERSPECTIVE }
            },
            .framebuffer = LoadRenderTexture(200, 200)
        }
    );
    gCoordinator.AddComponent(
        teapot,
        transform2D{ { 450, 150 } }
    );
    gCoordinator.AddComponent(
        teapot,
        render_box{ { 200, 200 }, WHITE }
    );

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{100.0f, 100.0f },  BLUE }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {50.0f, 400.0f } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, RADIO_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 50, 400, 100, 100 }
    );


    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{100.0f, 100.0f },  YELLOW }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {100.0f, 400.0f } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, OUTSIDE, MAZE_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 100, 150, 100, 100 }
    );

}