#include "Coordinator.hpp"
#include "LevelManager.h"

#include "Player.h"
#include "Core.h"

#include "RenderSystem.h"
#include "PlayerSystem.h"

extern Coordinator gCoordinator;


void LevelManager::register_components()
{
    gCoordinator.RegisterComponent<transform25D>();
    gCoordinator.RegisterComponent<player>();
    gCoordinator.RegisterComponent<render>();
}


void LevelManager::set_signatures()
{
    Signature sig;

    sig.set(gCoordinator.GetComponentType<player>());
    sig.set(gCoordinator.GetComponentType<transform25D>());
    gCoordinator.SetSystemSignature<PlayerSystem>(sig);

    sig.reset();
}

// until serialization is done
void LevelManager::place_objs()
{
    Entity en = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
        en,
        render{ Vector2{100.0f, 100.0f },  WHITE }
    );

    gCoordinator.AddComponent(
        en,
        transform25D{ Vector2 {200.0f, 200.0f } }
    );

    gCoordinator.AddComponent(
        en,
        player{}
    );

}