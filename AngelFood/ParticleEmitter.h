#pragma once

#include "Timer.hpp"
#include "raylib.h"

#include "Game.h"

struct Particle
{
	Vector2 position;
	Vector2 force;
	Vector2 velocity;
	Color color;
	float lifetime;
	float size;

	Vector2 initial_position;
	float initial_lifetime;
	unsigned char initial_alpha;

	bool active;

	Timer blink_time;
	Timer blink_fade;
	bool dimming;
};

enum emitter_type : int
{
	ET_JUMP,
	ET_FIREFLIES,
	ET_ITEM_PICKUP,
	ET_SPARK_PICKUP
};

struct particle_emitter
{
	size_t capacity;
	size_t alive_count;

	std::pair<Vector2, Vector2> offset;

	Color color;

	Vector2 initial_dir;
	float max_angle_variation;
	std::pair<float, float> initial_speed;
	std::pair<float, float> initial_lifetime;
	std::pair<float, float> initial_size;

	size_t num_per_emit;

	bool emitting;
	bool one_shot;

	ASSETS texture_id;

	Timer time_between_emit;

	emitter_type type;

	std::vector<Particle> particles;
};
