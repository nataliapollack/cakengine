#pragma once
#include "Events.hpp"

#define HEALTH_TICK 10

class EnergyManager
{
public:
    void init();

    void TakeEnergyDmg(Event& event);
    void ChargeEnergy(Event& event);

    void TakeHealthDmg(Event& event);

    void CheckEnergyLevels();
    void DrawEnergyLevels();
private:
    int TotalEnergyLeft;
    int TotalHealthLeft;
};