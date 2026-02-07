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
    StartFade();
}

SCREENS ScreenManager::GetScreen()
{
    return current_screen;
}

void ScreenManager::SetScreen(SCREENS screen)
{
    current_screen = screen;
}

void ScreenManager::UpdateFadeTransition(float dt)
{
    if (!IsFading)
        return;

    // Fade timer counts up for fade in and down for fade out
 /*   if (fadeTimerRemaining < 0)
    {
        FadeIn(dt);
    }*/
    if (fadeTimerRemaining >= 0)
    {
        FadeOut(dt);
    }
}

//void ScreenManager::FadeIn(float dt)
//{
//    fadeTimerRemaining += dt;
//
//    if (fadeTimerRemaining >= 0)
//    {
//        fadeTimerRemaining = fadeTimerDefault;
//        return;
//    }
//    Color fadeColor = BLACK;
//    fadeColor.a = 255 * (fadeTimerRemaining / fadeTimerDefault);
//    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeColor);
//}

void ScreenManager::FadeOut(float dt)
{
    fadeTimerRemaining -= dt;

    if (fadeTimerRemaining <= 0)
    {
        IsFading = false;
        return;
    }
    Color fadeColor = BLACK;
    fadeColor.a = 255 * (fadeTimerRemaining / fadeTimerDefault);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeColor);
}

bool ScreenManager::GetIsFading()
{
    return IsFading;
}

void ScreenManager::StartFade()
{
    fadeTimerRemaining = fadeTimerDefault;
    IsFading = true;
}
