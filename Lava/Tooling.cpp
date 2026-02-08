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
    gCoordinator.RegisterComponent<render_texture>();

    gCoordinator.RegisterComponent<viewport3D>();
    // TODO: temp? replace with asset manager ref?
    gCoordinator.RegisterComponent<model_view>();
    gCoordinator.RegisterComponent<ModelAnimationInfo>();
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
    auto& assetMngr = AssetManager::get();
    Entity en;

    /////////////////////// Environment Inside stuff /////////////////////////////////
    Texture2D baseTex = assetMngr.get<Texture2D>("base");
    baseTex.width = GetScreenWidth();
    baseTex.height = GetScreenHeight();
    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{1, 1 },  YELLOW }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {0, 0 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, ENVIRONMENT_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        render_texture(baseTex)
    );

    Texture2D baseOverlayTex = assetMngr.get<Texture2D>("baseoverlay");
    baseOverlayTex.width = GetScreenWidth();
    baseOverlayTex.height = GetScreenHeight();
    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{1, 1 },  YELLOW }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {0, 0 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, ENVIRONMENT_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        render_texture(baseOverlayTex)
    );

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{1, 1 },  YELLOW }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {360, 500 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, ENVIRONMENT_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        render_texture(assetMngr.get<Texture2D>("door"))
    );
 
    ////////////////////////////////////////////////////////////////
    // PLAYER
     
    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{50.0f, 50.0f },  WHITE }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {200.0f, 200.0f } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, PLAYER }
    );

    gCoordinator.AddComponent(
        en,
        player{ Vector2{30.0f, 30.0f} }
    );
    gCoordinator.AddComponent(
        en,
        collidble{50, 50, 50, 50}
    );
    gCoordinator.AddComponent(
        en,
        model_view{ .model = &assetMngr.get<Model>("agnus") }
    );
    gCoordinator.AddComponent(
        en,
        assetMngr.get<ModelAnimationInfo>("agnus")
    );
    gCoordinator.AddComponent(
        en,
        viewport3D{
            .view = {
                .position = { 0, 1, 15 },
                .target = { 0 },
                .up = { 0, 1, 0 },
                .fovy = { 30 },
                .projection = { CAMERA_PERSPECTIVE }
            },
            .framebuffer = LoadRenderTexture(200, 200)
        }
    );

    // 3D ENTITY BILLBOARD TEST
    // Doubt we want this test thing so commenting out - Braedan 
    //Entity teapot = gCoordinator.CreateEntity();
    //gCoordinator.AddComponent(
    //    teapot,
    //    model_view{ .model = &assetMngr.get<Model>("agnus") }
    //);
    //gCoordinator.AddComponent(
    //    teapot,
    //    viewport3D{
    //        .view = {
    //            .position = { 0, 1, 15 },
    //            .target = { 0 },
    //            .up = { 0, 1, 0 },
    //            .fovy = { 10 },
    //            .projection = { CAMERA_PERSPECTIVE }
    //        },
    //        .framebuffer = LoadRenderTexture(200, 200)
    //    }
    //);
    //gCoordinator.AddComponent(
    //    teapot,
    //    transform2D{ { 450, 150 } }
    //);
    //gCoordinator.AddComponent(
    //    teapot,
    //    render_box{ { 200, 200 }, WHITE }
    //);

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{50.0f, 100.0f },  BLUE }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {650.0f, 190.0f } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, RADIO_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 650.0f, 190.0f, 50, 100 }
    );

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{170.0f, 50.0f },  YELLOW }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {365, 500 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, MAZE_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 365, 500, 170.0f, 50 }
    );


    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{50.0f, 80.0f },  GREEN }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {155, 278 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, SOLAR_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 155, 278, 50.0f, 80.0f }
    );

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{80.0f, 30.0f },  RED }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {400, 200 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, CAM_OBJ }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 400, 200, 80, 30 }
    );

    /// WALLS
    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{170, 900 },  GRAY }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {0, 0 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, WALL }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 0, 0, 170, 900 }
    );



    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{170, 900 },  GRAY }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {786, 0 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, WALL }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 786, 0, 170, 900 }
    );

    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{900, 50 },  GRAY }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {0, 0 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, WALL }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 0, 0, 900, 100 }
    );


    en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render_box{ Vector2{900, 50 },  GRAY }
    );

    gCoordinator.AddComponent(
        en,
        transform2D{ Vector2 {0, 511 } }
    );

    gCoordinator.AddComponent(
        en,
        status{ true, true, INSIDE, WALL }
    );
    gCoordinator.AddComponent(
        en,
        collidble{ 0, 511, 900, 100 }
    );
}