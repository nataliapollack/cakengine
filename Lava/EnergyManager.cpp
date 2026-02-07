#pragma once
#include "EnergyManager.h"
#include "Coordinator.hpp"

#include "Lava.h"
#include "raylib.h"

extern Coordinator gCoordinator;

void EnergyManager::init()
{
    TotalEnergyLeft = 100;
    TotalHealthLeft = 100;
    ChargeAmount = 0;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Energy::ENERGY_DOWN, EnergyManager::TakeEnergyDmg));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Energy::ENERGY_UP, EnergyManager::ChargeEnergy));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Health::HEALTH_DMG, EnergyManager::TakeHealthDmg));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Time::NIGHT_BEGIN, EnergyManager::NightBegins));
}

void EnergyManager::TakeEnergyDmg(Event& event)
{
    int dmg_tick =  event.GetParam<int>(Events::Energy::ENERGY_TICK);


    if (TotalEnergyLeft <= 0)
    {
        TotalEnergyLeft = 0;
        TotalHealthLeft -= HEALTH_TICK;
    }
    else
    {
        TotalEnergyLeft -= dmg_tick;
    }
}

void EnergyManager::ChargeEnergy(Event& event)
{
    float energy_tick = event.GetParam<float>(Events::Energy::ENERGY_TICK);

   ChargingPanels = true;
   ChargeAmount += energy_tick;
}

void EnergyManager::CheckEnergyLevels()
{
    if (TotalHealthLeft <= 0)
    {
        Event game(Events::Game::END);
        gCoordinator.SendEvent(game);
    }
    if (ChargingPanels)
    {
        TotalEnergyLeft += 5;
    }
}

void EnergyManager::update(float dt)
{
    if (ChargingPanels)
    {
        TotalEnergyLeft += ChargeAmount / 60;
    }
}

void EnergyManager::NightBegins(Event& event)
{
    ChargingPanels = false;
    ChargeAmount = 0;
}

void EnergyManager::DrawEnergyLevels()
{
    DrawRectangle(5, 5, 160, 40, GRAY);
    DrawRectangle(10, 10, 150 * (TotalEnergyLeft / 100), 30, GREEN);
}

void EnergyManager::TakeHealthDmg(Event& event)
{
    TotalHealthLeft -= HEALTH_TICK;
}