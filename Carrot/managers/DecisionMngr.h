#pragma once

#include "Events.hpp"

#include "dialogue.h"
#include "raylib.h"

#include <vector>
#include <fstream>

enum decisions
{
    FIRST,
    SECOND,
    UNMADE
};


enum DOORS
{
    normal_door, 
    locked_door,
    exit_door
};

class DecisionMngr
{
public:
    void init();
    void update();

    void draw();

    bool GetDecisionStatus();
    void ResetDecisions();

    void TriggerDecision(Event& event);
    void TriggerConfirmation(Event& event);
    void TriggerLockedDoor(Event& event);
    void TriggerExit(Event& event);
    void TriggerNewDay(Event& event);

private:
    bool in_decision;
    bool in_confirmation;
    int decision;
    int door;
    int day;

    Rectangle box_one;
    Rectangle box_two;

    std::string decision_one;
    std::string decision_two;
};