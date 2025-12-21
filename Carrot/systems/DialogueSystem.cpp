#include "DialogueSystem.h"
#include "components/Collidble.h"
#include "Game.h"

#include <string>

#include "Coordinator.hpp"

extern Coordinator gCoordinator;

void DialogueSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Collision::OBJ_TYPE, DialogueSystem::TriggerDialogue));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Dialogue::DECISION, DialogueSystem::TriggerFollowupDialogue));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, DialogueSystem::TriggerNewDay));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::DOOROPEN, DialogueSystem::TriggerLock));

    in_convo = false;
    current_convo = 0;
    interact = true;
    day = 0;
}

void DialogueSystem::update()
{
    if (displaying_text && IsKeyPressed(KEY_ENTER))
    {
        if (current_file.eof())
        {
            if (current_convo == decision)
            {
                std::string lol = std::to_string(current_char) + std::to_string(current_convo);

                Event dec_happen(Events::Decision::DECISION);
                dec_happen.SetParam(Events::Decision::DECISION, atoi(lol.c_str()));
                gCoordinator.SendEvent(dec_happen);
            }

            in_convo = false;
            convo_list[current_char] = convo_list[current_char] == dialogue_list::extra ? dialogue_list::extra : convo_list[current_char] + 1;
        }
        std::getline(current_file, current_line);
        displaying_text = false;
    }
}

void DialogueSystem::TriggerDialogue(Event& event)
{
    if (interact)
    {
        in_convo = true;
        OBJ_TYPES type = event.GetParam<OBJ_TYPES>(Events::Collision::OBJ_TYPE);

        current_char = type - 1;
        current_convo = convo_list[current_char];

        current_file.close();
        std::string file_name = "dialogue/" + std::to_string(day) + "/" + std::to_string(current_char + 1) + std::to_string(convo_list[current_char]) + ".txt";
        current_file.open(file_name.c_str());
        std::getline(current_file, current_line);

        displaying_text = false;
    }
}

void DialogueSystem::draw()
{
    if (in_convo)
    {
     //   DrawText(current_line.c_str(), 0, 0, 40, PINK);
        displaying_text = true;


        Rectangle source = { 0,0, girlies[current_char].width, girlies[current_char].height};
        Vector2 origin = { 0.0f, 0.0f };
        Rectangle dest = { 0, 0, girlies[current_char].width * 0.15, girlies[current_char].height * 0.15};

        if (current_char == 1 || current_char == 3 )
            dest = { 0, 150, (float)girlies[current_char].width * 0.15f, (float)girlies[current_char].height * 0.15f };

        DrawTexturePro(girlies[current_char], source, dest, origin, 0.0f, WHITE);

        Rectangle box = Rectangle{ 50, GetScreenHeight() - 150.0f, GetScreenWidth() - 100.0f, 200 };

        DrawTextBoxedSelectable(subtitled, current_line.c_str(), box, 30.0f, 1.0f, true, WHITE, 0, current_line.size(), WHITE, ColorAlpha(BLACK, 0.75));
    }
}

bool DialogueSystem::GetDialogue()
{
    return in_convo;
}

void  DialogueSystem::TriggerFollowupDialogue(Event& event)
{
    in_convo = true;

    int dec = event.GetParam<int>(Events::Dialogue::DECISION);

    current_convo = convo_list[current_char];

    current_file.close();
    std::string file_name = "dialogue/" + std::to_string(day) + "/" + std::to_string(current_char + 1) + std::to_string(convo_list[current_char]) + std::to_string(dec) + ".txt";
    current_file.open(file_name.c_str());
    std::getline(current_file, current_line);

    convo_list[current_char] = convo_list[current_char] == dialogue_list::extra ? dialogue_list::extra : convo_list[current_char] + 1;
    displaying_text = false;
}

void  DialogueSystem::TriggerNewDay(Event& event)
{
    day++;

    for (int i = 0; i < 4; i++)
    {
        convo_list[i] = 0;
    }
}

void DialogueSystem::load()
{
  //  subtitled= VCR_OSD_MONO_1.001.ttf
    subtitled = LoadFont("art/fonts/VCR_OSD_MONO_1.001.ttf");

    girlies[0] = LoadTexture("art/1b.png");
    girlies[1] = LoadTexture("art/2b.png");
    girlies[2] = LoadTexture("art/3b.png");
    girlies[3] = LoadTexture("art/4b.png");
}

void DialogueSystem::unload()
{
    UnloadFont(subtitled);

    UnloadTexture(girlies[0]);
    UnloadTexture(girlies[1]);
    UnloadTexture(girlies[2]);
    UnloadTexture(girlies[3]);
}

void DialogueSystem::TriggerLock(Event &event)
{
    interact = false;
}