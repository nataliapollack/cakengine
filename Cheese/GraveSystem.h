#pragma once
#include "System.hpp"
#include "Events.hpp"

class GraveySystem : public System
{
public:
    void init();  // initialize all the money values / sentences
    void reset(); // to reset positiopns / values between levels

    void EmptiedOut(Event& event); // player sucessfully obtained money...
};