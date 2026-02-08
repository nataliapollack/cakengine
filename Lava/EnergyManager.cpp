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

    out_of_energy = false;
    sent_energy = false;

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
        out_of_energy = true;
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

void EnergyManager::update(float dt)
{
    if (ChargingPanels)
    {
        TotalEnergyLeft += ChargeAmount / 60;
    }

    if (TotalHealthLeft <= 0)
    {
        Event game(Events::Game::END);
        gCoordinator.SendEvent(game);
    }

    if (out_of_energy)
    {
        if (!sent_energy)
        {
            sent_energy = true;

            Event no_energy(Events::Energy::NO_ENERGY);
            gCoordinator.SendEvent(no_energy);
        }
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