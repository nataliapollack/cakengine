#pragma once

#define DAYS_TO_LAST 5
#define MAX_NIGHT_TIME 35
#define MAX_DAY_TIME 65

class CycleManager
{
public:
    void init();

    void UpdateTimer(float dt);
    bool NightOrDay();

    void draw();
private:
    bool night;
    int current_day;
    float current_time;
};