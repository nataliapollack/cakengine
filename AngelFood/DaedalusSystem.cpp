
#include "DaedalusSystem.h"
#include "Coordinator.hpp"

#include "AssetManager.h"
#include "Core.h"
#include "Events.hpp"

#include "ParticleEmitter.h"

#include "raymath.h"

#include <algorithm>
#include <iostream>

extern Coordinator gCoordinator;
extern AssetManager gAssetMngr;

void DaedalusSystem::init()
{
	move_timer = Timer(5.0f);
	curr_waypoint = -1;

	if (entities_list.size() > 0)
	{
		curr_waypoint = 0;
	}

	for (auto& entity : entities_list)
	{
		auto& way = gCoordinator.GetComponent<waypoint>(entity);
		if (way.index == curr_waypoint)
		{
			auto& transf = gCoordinator.GetComponent<transform2D>(entity);
			daedalus_position = transf.pos;
		}
	}

	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Collision::WAYPOINT, 
			DaedalusSystem::PlayerInRange)
	);

	{
		emitter = gCoordinator.CreateEntity();

		gCoordinator.AddComponent(emitter,
			status{ true, true, EMITTER });

		gCoordinator.AddComponent(emitter,
			transform2D{daedalus_position});

		Texture2D texture = gAssetMngr.GetAsset(DAEDALUS_FLOAT);

		Vector2 base_offset{ texture.width / 2.0f, texture.height / 2.0f };

		Vector2 min_offset{ base_offset.x - 5.0f, base_offset.y - 5.0f };
		Vector2 max_offset{ base_offset.x + 5.0f, base_offset.y + 5.0f };

		gCoordinator.AddComponent(emitter,
			particle_emitter{
				32,         // capacity
				0,           // alive count
				{ min_offset, max_offset },
				ColorAlpha(WHITE, 0.7f), // color
				Vector2Rotate(Vector2UnitY, DEG2RAD * -60.0f),
				120.0f, // init dir
				{ 350.0f, 600.0f },      // init speed
				{0.50f, 1.0f},        // init lifetime
				{5.0f, 10.0f}, // init size
				2,          // num per emit
				false ,        // emitting
				true,       // one shot effect
				COUNT,
				Timer(0.0f), // time between emits
				ET_JUMP,
				{}
			}
		);
	}

	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Scene::RESET, DaedalusSystem::reset)
	);
}

void DaedalusSystem::update(float dt)
{
	if (curr_waypoint == -1 && entities_list.size() > 0)
		curr_waypoint = 0;

	for (auto& entity : entities_list)
	{
		auto& way = gCoordinator.GetComponent<waypoint>(entity);

		if (curr_waypoint == way.index)
		{
			auto& transf = gCoordinator.GetComponent<transform2D>(entity);
			if (curr_waypoint == 0)
			{
				daedalus_position = transf.pos;
				last_position = transf.pos;
			}
			else
			{
				if (!move_timer.update(dt))
				{
					float percent = move_timer.count() / move_timer.time();
					if ((percent > 0.25f && percent < 0.3f) ||
						(percent > 0.5f && percent < 0.55f) ||
						(percent > 0.75f && percent < 0.8f))
					{
						auto& emit = gCoordinator.GetComponent<particle_emitter>(emitter);

						emit.emitting = true;
					}

					daedalus_position =
						Vector2Lerp(last_position, transf.pos, percent);	
					auto& emitter_transform = gCoordinator.GetComponent<transform2D>(emitter);
					emitter_transform.pos = daedalus_position;
				}
				else
				{
					if (!way.should_stop)
					{
						if (curr_waypoint == 1)
						{
							Event tutorial(Events::Text::WALK);
							gCoordinator.SendEvent(tutorial);
						}
						SetNewPosition();
					}
					else
					{
						Event flight_end(Events::Input::END_DAEDALUS_FLIGHT);
						gCoordinator.SendEvent(flight_end);

						if (curr_waypoint == 1)
						{
							Event tutorial(Events::Text::WALK);
							gCoordinator.SendEvent(tutorial);
						}
						else if (curr_waypoint == 3)
						{
							Event tutorial(Events::Text::DOUBLE);
							gCoordinator.SendEvent(tutorial);
						}
					}
				}
			}
		}
	}
}

void DaedalusSystem::draw()
{
	if (curr_waypoint == -1)
		return;

	Texture2D texture = gAssetMngr.GetAsset(DAEDALUS_IDLE);
	if (move_timer.is_running())
		texture = gAssetMngr.GetAsset(DAEDALUS_FLOAT);

	Rectangle source{ 0, 0, (float)texture.width, (float)texture.height };
	Rectangle dest{ daedalus_position.x, daedalus_position.y,
		(float)texture.width, (float)texture.height };

	DrawTexturePro(texture, source, dest, Vector2Zero(), 0.0f, WHITE);
}

void DaedalusSystem::PlayerInRange(Event& event)
{
	Entity hit_waypoint = 
		event.GetParam<Entity>(Events::Collision::WAYPOINT_DATA);

	auto& way = gCoordinator.GetComponent<waypoint>(hit_waypoint);

	if (way.index != curr_waypoint || 
		curr_waypoint == entities_list.size() - 1) return;

	SetNewPosition();

	Event flight_start(Events::Input::START_DAEDALUS_FLIGHT);
	gCoordinator.SendEvent(flight_start);
}

void DaedalusSystem::SetNewPosition()
{
	//last_position = daedalus_position;

	for (auto& entity : entities_list)
	{
		auto& way = gCoordinator.GetComponent<waypoint>(entity);

		if (way.index == curr_waypoint)
		{
			auto& t = gCoordinator.GetComponent<transform2D>(entity);
			last_position = t.pos;
			daedalus_position = t.pos;
		}
	}

	curr_waypoint = std::clamp(curr_waypoint + 1,
		0, static_cast<int>(entities_list.size()));

	move_timer.start();
}

void DaedalusSystem::reset(Event& event)
{
	curr_waypoint = 0;
	move_timer.reset();
}
