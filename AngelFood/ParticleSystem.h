#pragma once

#include "System.hpp"

#include "raylib.h"
#include "rlgl.h"

class ParticleSystem : public System
{
public:
	void init();
	void update(float dt);
	void draw();
private:
	void init_emitter(const Entity& entity);

	Shader instanceShader;
	rlRenderBatch batch;
};
