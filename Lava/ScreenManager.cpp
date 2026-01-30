#pragma once
#include "ScreenManager.h"
#include "Coordinator.hpp"

#include "raylib.h"
#include "Lava.h"

extern Coordinator gCoordinator;

void ScreenManager::init()
{
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Game::SCREEN_CHANGE, ScreenManager::ChangeScreen));
}

void ScreenManager::ChangeScreen(Event& event)
{
    SCREENS screen = event.GetParam<SCREENS>(Events::Game::SCREEN_ID);
    current_screen = screen;
}

SCREENS ScreenManager::GetScreen()
{
    return current_screen;
}

void ScreenManager::SetScreen(SCREENS screen)
{
    current_screen = screen;
}