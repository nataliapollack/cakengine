
#include "DaedalusSystem.h"
#include "Coordinator.hpp"

#include "AssetManager.h"
#include "Core.h"
#include "Events.hpp"

#include "raymath.h"

#include <algorithm>

extern Coordinator gCoordinator;
extern AssetManager gAssetMngr;

void DaedalusSystem::init()
{
	move_timer = Timer(1.0f);
	curr_waypoint = -1;

	if (entities_list.size() > 0)
		curr_waypoint = 0;

	for (auto& entity : entities_list)
	{
	}

	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Collision::WAYPOINT, 
			DaedalusSystem::PlayerInRange)
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
			}
			else
			{
				if (!move_timer.update(dt))
				{
					daedalus_position =
						Vector2Lerp(last_position, transf.pos,
							move_timer.count() / move_timer.time());
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

	Rectangle source{ 0, 0, texture.width, texture.height };
	Rectangle dest{ daedalus_position.x, daedalus_position.y,
		texture.width, texture.height };

	DrawTexturePro(texture, source, dest, Vector2Zero(), 0.0f, WHITE);
}

void DaedalusSystem::PlayerInRange(Event& event)
{
	Entity hit_waypoint = 
		event.GetParam<Entity>(Events::Collision::WAYPOINT_DATA);

	auto& way = gCoordinator.GetComponent<waypoint>(hit_waypoint);

	if (way.index != curr_waypoint) return;

	last_position = daedalus_position;

	curr_waypoint = std::clamp(curr_waypoint + 1, 
		0, static_cast<int>(entities_list.size()));

	move_timer.start();
}
