#include "managers/InnerDialogueMngr.h"

#include "Coordinator.hpp"

#include "string.h"
#include "raylib.h"
#include "Game.h"

extern Coordinator gCoordinator;

void InnerDialogue::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, InnerDialogue::TriggerNewDay));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Thoughts::THINK, InnerDialogue::TriggerThoughts));
    day = 0;
    new_day = false;
    in_thoughts = false;
    displaying_text = false;
    thought_counter = 0;
}

void InnerDialogue::update()
{
    if (in_thoughts)
    {
        if (displaying_text && IsKeyPressed(KEY_ENTER))
        {
            if (current_file.eof() || !current_file.good())
            {
                in_thoughts = false;

                if (new_day)
                {
                    new_day = false;
                    Event day(Events::Transition::FADEIN);
                    gCoordinator.SendEvent(day);
                }
            }

            std::getline(current_file, current_line);
            displaying_text = false;
        }
    }
}

void InnerDialogue::draw()
{
    if (in_thoughts)
    {

        Rectangle box = Rectangle{ 50, GetScreenHeight() - 100.0f, GetScreenWidth() - 100.0f, 200 };

        DrawTextBoxedSelectable(GetFontDefault(), current_line.c_str(), box, 30.0f, 1.0f, true, WHITE, 0, current_line.size(), WHITE, ColorAlpha(BLACK, 0.75));
        displaying_text = true;

    }
}

void InnerDialogue::TriggerThoughts(Event& event)
{
    in_thoughts = true;

    current_file.close();
    std::string file_name = "dialogue/" + std::to_string(thought_counter) + std::to_string(day) +  "t.txt";
    current_file.open(file_name.c_str());
    thought_counter += 1;
    std::getline(current_file, current_line);

    displaying_text = false;
}

void InnerDialogue::TriggerNewDay(Event& event)
{
    day++;
    in_thoughts = true;
    new_day = true;

    current_file.close();
    std::string file_name = "dialogue/0d" + std::to_string(day) + ".txt";
    current_file.open(file_name.c_str());
    std::getline(current_file, current_line);

    displaying_text = false;
}

bool InnerDialogue::GetThoughtsStatus()
{
    return in_thoughts;
}
