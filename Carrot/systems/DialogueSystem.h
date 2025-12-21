#pragma once

#include "System.hpp"
#include "Events.hpp"

#include "dialogue.h"

#include "raylib.h"

#include <vector>
#include <fstream>

class DialogueSystem
{
public:
    void init();
    void update();

    void load();
    void unload();

    void draw();

    bool GetDialogue();

    void TriggerDialogue(Event& event);

    void TriggerFollowupDialogue(Event& event);

    void TriggerNewDay(Event& event);
    void TriggerLock(Event& event);

private:
    bool in_convo;
    bool interact;
    bool displaying_text;

    int day;
    int current_convo;
    int current_char;

    int convo_list[4] = { 0 };

    std::string current_line;
    std::fstream current_file;

    Texture girlies[4];
    Font subtitled;
};