
#include "Coordinator.hpp"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Core.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "AssetManager.h"

extern Coordinator gCoordinator;
extern Camera2D gCamera;
extern AssetManager gAssetMngr;

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

	init_update_functions();
	init_draw_functions();
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
		if (emit.type == ET_FIREFLIES)
			position -= transf.pos;

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
			//emit.update_function(emit, par, dt);

			update_functions.at(emit.type)(emit, par, dt);

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
		auto& transf = gCoordinator.GetComponent<transform2D>(entity);

		if (emit.particles.size() < emit.capacity)
			init_emitter(entity);

		for (size_t i = 0; i < emit.alive_count; ++i)
		{
			auto& par = emit.particles.at(i);
			// TODO: draw particle code

			draw_functions.at(emit.type)(emit, transf, par);

			//emit.color_curve(emit, par);

			//if (emit.texture.id != 0)
			//{
			//	auto& texture = emit.texture;
			//	// Draw texture
			//	DrawTexture(texture, par.position.x - texture.width / 2.0f, 
			//		par.position.y - texture.height / 2.0f, WHITE);
			//}
			//else
			//{
			//	DrawCircle(par.position.x + par.size, 
			//		par.position.y + par.size,
			//		par.size, par.color);
			//}
		}
	}

	rlDrawRenderBatchActive();
	rlSetRenderBatchActive(nullptr);
	EndShaderMode();
}

void ParticleSystem::init_update_functions()
{
	update_functions.insert({
	ET_JUMP,
	[](particle_emitter& emit, Particle& par, float dt)
	{
		par.velocity = Vector2Scale(par.velocity, 0.9f);

		par.position = Vector2Add(par.position,
			Vector2Scale(par.velocity, dt));
	}
		});

	update_functions.insert({
		ET_FIREFLIES,
		[](particle_emitter& emit, Particle& par, float dt)
		{
			Vector2 dir = Vector2Subtract(
				par.initial_position, par.position);
			float dist = Vector2Length(dir);

				par.force = Vector2Scale(dir, dist);

				par.velocity += par.force * dt * dt;

				if (GetRandomValue(0, 1))
				{
					par.velocity += Vector2Scale(
						{ par.velocity.y, -par.velocity.x },
						0.25f
					);
				}
				else
				{
					par.velocity += Vector2Scale(
						{ -par.velocity.y, par.velocity.x },
						0.25f
					);
				}

				par.velocity = Vector2ClampValue(
					par.velocity, -100.0f, 100.0f);

				par.position = Vector2Add(par.position,
					Vector2Scale(par.velocity, dt));
		}
		});

	update_functions.insert({
		ET_ITEM_PICKUP,
		[](particle_emitter& emit, Particle& par, float dt)
		{
			par.velocity += { 0.0f, 60000.0f * dt * dt };

			par.position = Vector2Add(par.position,
				Vector2Scale(par.velocity, dt));
		}
		});
}


void ParticleSystem::init_draw_functions()
{
	draw_functions.insert({
		ET_JUMP,
		[](particle_emitter& emit, transform2D& transf, Particle& par)
		{
			par.color.a = static_cast<float>(par.initial_alpha) *
				(par.lifetime / par.initial_lifetime);

			
			//if (emit.texture_id != ASSETS::COUNT)
			//{
			//	Texture2D texture = gAssetMngr.GetAsset(emit.texture_id);
			//	// Draw texture
			//	DrawTexture(texture, par.position.x - texture.width / 2.0f,
			//		par.position.y - texture.height / 2.0f, WHITE);
			//}
			//else
			{
				DrawCircle(par.position.x + par.size,
					par.position.y + par.size,
					par.size, par.color);
			}
		}
	});

	draw_functions.insert({
		ET_FIREFLIES,
		[](particle_emitter& emit, transform2D& transf, Particle& par)
		{
			//if (emit.texture_id != ASSETS::COUNT)
			//{
			//	Texture2D texture = gAssetMngr.GetAsset(emit.texture_id);
			//	// Draw texture
			//	DrawTexture(texture, par.position.x - texture.width / 2.0f,
			//		par.position.y - texture.height / 2.0f, WHITE);
			//}
			//else
			{
				DrawCircle(
					par.position.x + par.size + transf.pos.x,
					par.position.y + par.size + transf.pos.y,
					par.size / 2.0f, par.color);
				DrawCircleGradient(
					par.position.x + par.size + transf.pos.x,
					par.position.y + par.size + transf.pos.y,
					par.size * 2.0f, 
					ColorAlpha(YELLOW, 0.5f), ColorAlpha(YELLOW, 0.f));
			}
		}
	});

	draw_functions.insert({
		ET_ITEM_PICKUP,
		[](particle_emitter& emit, transform2D& transf, Particle& par)
		{
			par.color.a = static_cast<float>(par.initial_alpha) *
				(par.lifetime / par.initial_lifetime);

			//if (emit.texture_id != ASSETS::COUNT)
			//{
			//	Texture2D texture = gAssetMngr.GetAsset(emit.texture_id);
			//	// Draw texture
			//	DrawTexture(texture, par.position.x - texture.width / 2.0f,
			//		par.position.y - texture.height / 2.0f, WHITE);
			//}
			//else
			{
				DrawCircle(par.position.x + par.size,
					par.position.y + par.size,
					par.size, par.color);
			}
		}
	});
}
