#include "SolarPanels.h"
#include "Coordinator.hpp"
#include "raylib.h"

extern Coordinator gCoordinator;

void SolarPanel::update()
{

}

void SolarPanel::draw()
{

}

bool SolarPanel::active()
{
    return toggle;
}

void SolarPanel::StartMinigame(Event& event)
{
    toggle = true;
}

void SolarPanel::StartNewDay(Event& event)
{
    toggle = false;
    completed = false;
}