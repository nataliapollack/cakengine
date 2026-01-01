#pragma once

#include "Timer.hpp"

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
};

struct particle_emitter
{
	float capacity;
	float alive_count;

	std::pair<Vector2, Vector2> offset;

	Color color;

	std::pair<Vector2, Vector2> initial_dir;
	std::pair<float, float> initial_speed;
	std::pair<float, float> initial_lifetime;
	std::pair<float, float> initial_size;

	size_t num_per_emit;

	bool emitting;
	bool one_shot;

	Timer time_between_emit;

	std::function<void(particle_emitter&, Particle&, float)> update_function;
	std::function<void(particle_emitter&, Particle&)> color_curve;

	std::vector<Particle> particles;
};
