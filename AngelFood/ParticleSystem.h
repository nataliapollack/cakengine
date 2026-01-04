#pragma once

#include "System.hpp"

#include "raylib.h"
#include "rlgl.h"
#include "ParticleEmitter.h"
#include "Core.h"

#include <unordered_map>
#include <functional>

class ParticleSystem : public System
{
public:
	void init();
	void update(float dt);
	void draw();
private:
	void init_emitter(const Entity& entity);

	void init_update_functions();
	void init_draw_functions();

	Shader instanceShader;
	rlRenderBatch batch;

	std::unordered_map<emitter_type, 
		std::function<void(particle_emitter&, Particle&, float)>> 
		update_functions;

	std::unordered_map<emitter_type,
		std::function<void(particle_emitter&, transform2D&, Particle&)>>
		draw_functions;
};
