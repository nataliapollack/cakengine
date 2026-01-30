#pragma once
#include "Events.hpp"

class EnergyManager
{
public:
    void init();

    void TakeEnergyDmg(Event& event);
    void ChargeEnergy(Event& event);

    void CheckEnergyLevels();
    void DrawEnergyLevels();
private:
    int TotalEnergyLeft;
};