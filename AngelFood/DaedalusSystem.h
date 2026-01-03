#pragma once

#include "System.hpp"

#include "Timer.hpp"
#include "Events.hpp"

#include "raylib.h"

class DaedalusSystem : public System
{
public:
	void init();
	void update(float dt);
	void draw();

private:
	void PlayerInRange(Event& event);
	void SetNewPosition();

	Vector2 daedalus_position;
	Vector2 last_position;

	Timer move_timer;

	int emitter;
	int curr_waypoint;
};
