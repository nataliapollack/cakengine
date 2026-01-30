#include "Tooling.h"

#include "Coordinator.hpp"

#include "Lava.h"
#include "raylib.h"

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
    // TODO: TEMP! REPLACE WITH ASSET REF WHEN ASSET MANAGER IMPLEMENTED
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