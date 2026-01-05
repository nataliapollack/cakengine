#include "TextManager.h"
#include "Coordinator.hpp"

#include "Game.h"

#include <iostream>

extern Coordinator gCoordinator;

void TextManager::init()
{
    time_shown = 7.0f;

    frame_counter = 0;
    currently_showing = false;

    text_font = LoadFont("art/WalterTurncoat-Regular.ttf");

    current_string[0] = "...walk using the arrow keys...";
    current_string[1] = "...glide holding shift mid-air...";
    current_string[2] = "...flap your wings by hitting space...";
    current_string[3] = "...double jump now unlocked...";

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Text::WALK,
            TextManager::TriggerWalkingTutorial));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Text::DOUBLE,
            TextManager::TriggerWalkingTutorial));
}

void TextManager::update()
{
    if (currently_showing)
    {
        if (frame_counter >= 60 * time_shown)
        {
            frame_counter = 0;

            string_counter++;

            if (string_counter >= 3)
            {
                currently_showing = false;
                string_counter = 3;
            }
        }
        frame_counter++;
    }
}

void TextManager::draw()
{
    if (currently_showing)
    {
        DrawTextEx(text_font, current_string[string_counter].c_str(), 
            Vector2{150, 500}, 48.0f, 5.0f, WHITE);
    }
}

void TextManager::TriggerWalkingTutorial(Event& event)
{
    currently_showing = true;
    frame_counter = 0;
}

void TextManager::TriggerDoubleJumpTutorial(Event& event)
{
    currently_showing = true;
    frame_counter = 0;
}

void TextManager::TriggerCredits(Event& event)
{
    credits = true;
}

void TextManager::unload()
{
    UnloadFont(text_font);
}