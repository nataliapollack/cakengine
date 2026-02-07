#pragma once

struct enemy_state {
	float speed;

	struct {
		float fleeSpeedMult;
		float lightExposureTime = 0;
		bool isFleeing = false;
	} tower;
};
