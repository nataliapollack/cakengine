#include "TransitionMngr.h"

#include "Coordinator.hpp"
#include "Events.hpp"

#include "Game.h"

extern Coordinator gCoordinator;

void TransitionManager::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::FADEIN, TransitionManager::TriggerFadeIn));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::FADEOUT, TransitionManager::TriggerFadeOut));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, TransitionManager::TriggerDay));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::FADEWHITE, TransitionManager::TriggerWhite));

    fade_in = false;
    fade_out = false;
    fade_white = false;
    game_done = false;
    new_day = false;
}

void TransitionManager::init_in()
{
   fade_timer = 5.0f;
   current_fade = 0.0f;
}

void TransitionManager::init_out()
{
    fade_timer = 5.0f;
    current_fade = 1.0f;
}


void TransitionManager::update()
{
    if (fade_in)
    {
        update_fade_in();
    }
    if (fade_out)
    {
        update_fade_out();
    }
    if (fade_white)
    {
        update_fade_white();
    }
}

void TransitionManager::update_fade_in()
{
    if (current_fade >= 1.0)
    {
        current_fade = 1.0f;
    }
     
    if (current_frame >= 60.0f / 2.0f)
    {
        current_frame = 0;
        fade_timer  -= 1.0f;
    }

    current_fade += 0.05;
    current_frame++;

    if (fade_timer <= 0.0f)
    {
        // change here?
        current_fade = 1.0f;

        if (new_day)
        {
            fade_out = true;
            init_out();
            new_day = false;

            Event game(Events::Minigame::END);
            gCoordinator.SendEvent(game);
            fade_in = false;
        }
        else
        {
            Event fini(Events::Transition::CHANGEROOM);
            gCoordinator.SendEvent(fini);
            fade_in = false;
        }
    }
}

void TransitionManager::update_fade_out()
{
    if (current_fade <= 0.0)
    {
        current_fade = 0.0f;
    }

    if (current_frame >= 60.0f / 2.0f)
    {
        current_frame = 0;
        fade_timer -= 1.0f;
    }

    current_frame++;
    current_fade -= 0.05;

    if (fade_timer <= 0.0f)
    {
        current_fade = 0.0f;
        fade_out = false;
    }
}

void TransitionManager::draw_fade()
{
    if (fade_in || fade_out)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, current_fade));
    }
    if (fade_white)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(WHITE, current_fade));
    }
}

void TransitionManager::TriggerFadeIn(Event& event)
{
    fade_in = true;
    init_in();
}

void TransitionManager::TriggerFadeOut(Event& event)
{
    fade_out = true;
    init_out();
}

void TransitionManager::TriggerDay(Event& event)
{
    new_day = true;
}


void TransitionManager::TriggerWhite(Event& event)
{
    fade_white = true;
    init_in();
}

void TransitionManager::update_fade_white()
{
    if (current_fade >= 1.0)
    {
        current_fade = 1.0f;
    }

    if (current_frame >= 60.0f / 2.0f)
    {
        current_frame = 0;
        fade_timer -= 1.0f;
    }

    current_fade += 0.05;
    current_frame++;

    if (fade_timer <= 0.0f)
    {
        game_done = true;
    }
}

bool TransitionManager::GetGanmeOver()
{
    return game_done;
}