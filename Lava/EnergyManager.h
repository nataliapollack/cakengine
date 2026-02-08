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
    void NightBegins(Event& event);

    void CheckEnergyLevels();
    void DrawEnergyLevels();

    void update(float dt);

private:
    bool ChargingPanels;
    bool out_of_energy;
    bool sent_energy;

    int TotalEnergyLeft;
    int TotalHealthLeft;

    int ChargeAmount;
};