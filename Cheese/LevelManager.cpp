#include "Coordinator.hpp"
#include "LevelManager.h"

#include "Player.h"
#include "Render.h"
#include "Transform.h"


#include "RenderSystem.h"
#include "PlayerMovementSystem.h"

extern Coordinator gCoordinator;


void LevelManager::register_components()
{
    gCoordinator.RegisterComponent<Transform2D>();
    gCoordinator.RegisterComponent<Player>();
    gCoordinator.RegisterComponent<RenderBox>();
}


void LevelManager::set_signatures()
{
    Signature sig;
    sig.set(gCoordinator.GetComponentType<Player>());
    sig.set(gCoordinator.GetComponentType<Transform2D>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<Transform2D>());
    sig.set(gCoordinator.GetComponentType<RenderBox>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);
}

void LevelManager::place_objs()
{
    Entity player_en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        player_en,
        RenderBox{ 50, 50, WHITE }
    );

    gCoordinator.AddComponent(
        player_en,
        Transform2D{ Vector2 {200.0f, 200.0f } }
    );

    gCoordinator.AddComponent(
        player_en,
        Player{}
    );

    Entity bqackground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        bqackground,
        RenderBox{ (float)GetScreenWidth(), (float)GetScreenHeight(), DARKBLUE}
    );
}