
#include "Events.hpp"

#include <string>
#include <fstream>

class InnerDialogue
{
public:
    void init();
    void update();
    void draw();

    void TriggerThoughts(Event& event);
    void TriggerNewDay(Event& event);

    bool GetThoughtsStatus();

private:
    bool in_thoughts;
    bool new_day;
    bool displaying_text;
    int day;
    int thought_counter;

    std::string current_line;
    std::fstream current_file;
};