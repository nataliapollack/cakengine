
#include "Coordinator.hpp"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Core.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

extern Coordinator gCoordinator;
extern Camera2D gCamera;

static float getRandomFloat()
{
	return static_cast<float>(GetRandomValue(0, INT_MAX)) / 
		static_cast<float>(INT_MAX);
}

static float getRandomFloat(float min, float max)
{
	return min + (max - min) * getRandomFloat();
}

void ParticleSystem::init()
{
	instanceShader = LoadShader("shaders/particle2d.vs", nullptr);
	batch = rlLoadRenderBatch(1, 36);
}

void ParticleSystem::init_emitter(const Entity& entity)
{
	auto& emit = gCoordinator.GetComponent<particle_emitter>(entity);
	auto& transf = gCoordinator.GetComponent<transform2D>(entity);

	// Creating particle objects
	for (size_t i = 0; i < emit.capacity; ++i)
	{
		float speed = getRandomFloat(emit.initial_speed.first,
			emit.initial_speed.second);
		Vector2 dir = Vector2Rotate(emit.initial_dir, getRandomFloat(0.0f,
			DEG2RAD * emit.max_angle_variation));

		float lifetime = getRandomFloat(emit.initial_lifetime.first,
			emit.initial_lifetime.second);

		bool is_active = (i < emit.alive_count) ? true : false;

		Vector2 offset = {
			getRandomFloat(emit.offset.first.x, emit.offset.second.x),
			getRandomFloat(emit.offset.first.y, emit.offset.second.y)
		};
		Vector2 position = Vector2Add(transf.pos, offset);

		emit.particles.emplace_back(Particle{
			position,
			Vector2Zero(),
			Vector2Scale(dir, speed),
			emit.color,
			lifetime,
			getRandomFloat(emit.initial_size.first,
				emit.initial_size.second),
			position,
			lifetime,
			emit.color.a,
			is_active
		});
	}
}

void ParticleSystem::update(float dt)
{
	for (auto& entity : entities_list)
	{
		auto& emit = gCoordinator.GetComponent<particle_emitter>(entity);
		auto& transf = gCoordinator.GetComponent<transform2D>(entity);

		auto& p_list = emit.particles;

		if (p_list.size() < emit.capacity)
			init_emitter(entity);

		// Spawn new particles
		if (emit.emitting && emit.alive_count < emit.capacity)
		{
			if (!emit.time_between_emit.is_running())
				emit.time_between_emit.start();

			if (emit.time_between_emit.update(dt))
			{
				for (size_t i = 0; i < emit.num_per_emit; ++i)
				{
					if (emit.alive_count >= emit.capacity) break;

					float speed = getRandomFloat(emit.initial_speed.first,
						emit.initial_speed.second);

					Vector2 dir = Vector2Rotate(
						emit.initial_dir, getRandomFloat(0.0f,
						DEG2RAD * emit.max_angle_variation)
					);

					float lifetime = getRandomFloat(
						emit.initial_lifetime.first,
						emit.initial_lifetime.second);

					Vector2 offset
					{
						getRandomFloat(
							emit.offset.first.x, emit.offset.second.x),
						getRandomFloat(
							emit.offset.first.y, emit.offset.second.y)
					};
					Vector2 position = Vector2Add(transf.pos, offset);

					auto& next_particle = p_list.at(emit.alive_count);
					next_particle = Particle{
						position,
						Vector2Zero(),
						Vector2Scale(dir, speed),
						emit.color,
						lifetime,
						getRandomFloat(emit.initial_size.first,
							emit.initial_size.second),
						position,
						lifetime,
						emit.color.a,
						true
					};
					emit.alive_count += 1;
					emit.time_between_emit.start();
				}
				if (emit.one_shot)
				{
					emit.emitting = false;
				}
			}
		}

		// Update living particles
		for (size_t i = 0; i < emit.alive_count; ++i)
		{
			auto& par = p_list.at(i);
			if (!par.active) continue;

			// TODO: update particle code
			emit.update_function(emit, par, dt);

			par.lifetime -= dt;
			if (par.lifetime <= 0.0f)
			{
				par.active = false;

				emit.alive_count -= 1;
				std::swap(p_list.at(i), p_list.at(emit.alive_count));

				i -= 1;
			}
		}
	}
}

void ParticleSystem::draw()
{
	BeginShaderMode(instanceShader);
	rlSetRenderBatchActive(&batch);

	for (auto& entity : entities_list)
	{
		auto& emit = gCoordinator.GetComponent<particle_emitter>(entity);

		if (emit.particles.size() < emit.capacity)
			init_emitter(entity);

		for (size_t i = 0; i < emit.alive_count; ++i)
		{
			auto& par = emit.particles.at(i);
			// TODO: draw particle code

			emit.color_curve(emit, par);

			if (emit.texture.id != 0)
			{
				auto& texture = emit.texture;
				// Draw texture
				DrawTexture(texture, par.position.x - texture.width / 2.0f, 
					par.position.y - texture.height / 2.0f, WHITE);
			}
			else
			{
				DrawCircle(par.position.x + par.size, 
					par.position.y + par.size,
					par.size, par.color);
			}
		}
	}

	rlDrawRenderBatchActive();
	rlSetRenderBatchActive(nullptr);
	EndShaderMode();
}
