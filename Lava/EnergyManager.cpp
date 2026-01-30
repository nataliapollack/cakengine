#pragma once
#include "EnergyManager.h"
#include "Coordinator.hpp"

#include "Lava.h"
#include "raylib.h"

extern Coordinator gCoordinator;

void EnergyManager::init()
{
    TotalEnergyLeft = 100;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Energy::ENERGY_DOWN, EnergyManager::TakeEnergyDmg));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Energy::ENERGY_UP, EnergyManager::ChargeEnergy));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Health::HEALTH_DMG, EnergyManager::ChargeEnergy));
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
    int charge_tick = event.GetParam<int>(Events::Energy::ENERGY_TICK);

    TotalEnergyLeft += charge_tick;
}

void EnergyManager::CheckEnergyLevels()
{
    if (TotalHealthLeft <= 0)
    {
        Event game(Events::Game::END);
        gCoordinator.SendEvent(game);
    }
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