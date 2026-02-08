#pragma once

#include "Events.hpp"
#include <fstream>

class Dialogue
{
public:
    void init();
    void TriggerTutorial(Event& event);
    void TriggerDialgue(Event& event);

    bool GetStatus();
    void update();
    void draw();
private:
    bool in_dialogue;
    bool displaying_text;

    bool tutorials[5];

    std::string current_line;
    std::fstream current_file;
};