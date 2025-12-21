#include "DecisionMngr.h"
#include "components/Collidble.h"
#include "Game.h"

#include <string>

#include "Coordinator.hpp"

extern Coordinator gCoordinator;

void DecisionMngr::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Decision::DECISION, DecisionMngr::TriggerDecision));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, DecisionMngr::TriggerNewDay));

    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Confirmation::CONFIRM, DecisionMngr::TriggerConfirmation));


    decision = UNMADE;
    in_decision = false;
    day = 0;
    in_confirmation = false;

    float width = GetScreenWidth() / 2.0f;
    box_one = { 20, GetScreenHeight() - 200.0f, 400, 100};
    box_two = { 450, GetScreenHeight() - 200.0f, 400, 100 };
}

void DecisionMngr::TriggerDecision(Event& event)
{
    int file = event.GetParam<int>(Events::Decision::DECISION);

    std::string file_name = "dialogue/decisions/" + std::to_string(day) + "/" + std::to_string(file) + "dec.txt";
    std::fstream current_file(file_name.c_str());

    std::getline(current_file, decision_one);
    std::getline(current_file, decision_two);

    in_decision = true;
}

void DecisionMngr::TriggerConfirmation(Event& event)
{
    int door_type = event.GetParam<int>(Events::Confirmation::CONFIRM);

    if (door_type == 5)
    {
        door = normal_door;
        //std::string file_name = "dialogue/" + std::to_string(day) + "conf.txt";
        std::string file_name = "dialogue/decisions/" + std::to_string(day)  + "dconf.txt";
        std::fstream current_file(file_name.c_str());

        std::getline(current_file, decision_one);
        std::getline(current_file, decision_two);
    }
    if (door_type == 6)
    {
        TriggerLockedDoor(event);
    }
    if (door_type == 7)
    {
        TriggerExit(event);
    }

    in_confirmation = true;
}

bool DecisionMngr::GetDecisionStatus()
{
    return in_decision || in_confirmation;
}

void DecisionMngr::update()
{
    if (in_decision)
    {
        Vector2 mouse_pos = GetMousePosition();

        if (CheckCollisionPointRec(mouse_pos, box_one))
        {
            decision = FIRST;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Event dec_happen(Events::Dialogue::DECISION);
                dec_happen.SetParam(Events::Dialogue::DECISION, decision);
                gCoordinator.SendEvent(dec_happen);

                Event audio(Events::Audio::SOUND);
                audio.SetParam<int>(Events::Audio::SOUND, audience_ha);
                gCoordinator.SendEvent(audio);

                ResetDecisions();
            }
        }
        if (CheckCollisionPointRec(mouse_pos, box_two))
        {
            decision = SECOND;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Event dec_happen(Events::Dialogue::DECISION);
                dec_happen.SetParam(Events::Dialogue::DECISION, decision);
                gCoordinator.SendEvent(dec_happen);

                ResetDecisions();
            }
        }
    }

    if (in_confirmation)
    {
        Vector2 mouse_pos = GetMousePosition();

        if (CheckCollisionPointRec(mouse_pos, box_one))
        {
            decision = FIRST;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // yes
                if (door == normal_door)
                {
                    Event dec_happen(Events::Transition::FADEIN);
                    gCoordinator.SendEvent(dec_happen);
                    ResetDecisions();
                }
                if (door == locked_door)
                {
                  //  if (door == locked_door)
                    {
                        Event dec_happen(Events::Transition::DOOROPEN);
                        gCoordinator.SendEvent(dec_happen);
                        ResetDecisions();
                    }
                }

                if (door == exit_door)
                {
                    Event dec_happen(Events::Transition::FADEWHITE);
                    gCoordinator.SendEvent(dec_happen);
                    ResetDecisions();

                    Event audio(Events::Audio::SOUND);
                    audio.SetParam<int>(Events::Audio::SOUND, audience_app);
                    gCoordinator.SendEvent(audio);
                }
            }
        }
        if (CheckCollisionPointRec(mouse_pos, box_two))
        {
            decision = SECOND;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // no

                if (door == exit_door)
                {
                    Event dec_happen(Events::Transition::FADEWHITE);
                    gCoordinator.SendEvent(dec_happen);
                    ResetDecisions();

                    Event audio(Events::Audio::SOUND);
                    audio.SetParam<int>(Events::Audio::SOUND, audience_app);
                    gCoordinator.SendEvent(audio);
                }

                ResetDecisions();
            }
        }
    }
}

void DecisionMngr::draw()
{
    if (in_confirmation || in_decision)
    {
        Vector2 pos = { box_one.x, box_one.y };
        Color selected = decision == FIRST ? LIGHTGRAY : GRAY;

        DrawRectangleRec(box_one, selected);

        selected = decision == SECOND ? LIGHTGRAY : GRAY;

        DrawRectangleRec(box_two, selected);

      //  Rectangle box = Rectangle{ 50, GetScreenHeight() - 100.0f, GetScreenWidth() - 100.0f, 200 };
        DrawTextBoxedSelectable(GetFontDefault(), decision_one.c_str(), box_one, 30.0f, 1.0f, true, WHITE, 0, 0, WHITE, ColorAlpha(BLACK, 0.75));

        DrawTextBoxedSelectable(GetFontDefault(), decision_two.c_str(), box_two, 30.0f, 1.0f, true, WHITE, 0, 0, WHITE, ColorAlpha(BLACK, 0.75));
    }
}

void DecisionMngr::ResetDecisions()
{
    decision = UNMADE;
    in_decision = false;
    in_confirmation = false;
}

void DecisionMngr::TriggerNewDay(Event& event)
{
    day += 1;
}

void DecisionMngr::TriggerLockedDoor(Event& event)
{
    //std::string file_name = "dialogue/" + std::to_string(day) + "conf.txt";
    std::string file_name = "dialogue/decisions/" + std::to_string(day) + "lconf.txt";
    std::fstream current_file(file_name.c_str());

    std::getline(current_file, decision_one);
    std::getline(current_file, decision_two);

    in_confirmation = true;
    door = locked_door;
}

void DecisionMngr::TriggerExit(Event& event)
{
    //std::string file_name = "dialogue/" + std::to_string(day) + "conf.txt";
    std::string file_name = "dialogue/decisions/" + std::to_string(day) + "econf.txt";
    std::fstream current_file(file_name.c_str());

    std::getline(current_file, decision_one);
    std::getline(current_file, decision_two);

    in_confirmation = true;
    door = exit_door;
}