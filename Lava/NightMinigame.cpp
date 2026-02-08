#include "NightMinigame.h"

#include <raymath.h>
#include <ctime>
#include <cstdlib>
#include <cfloat>

#include "Coordinator.hpp"
#include "ScreenManager.h"

extern Coordinator gCoordinator;

namespace UI {
static constexpr Vector2 CAM_RECT_DIMS = { 70, 70 };
static constexpr int CAM_RECT_SPACING = 10;
static constexpr int FONT_SIZE = 24;
static constexpr Color CAM_RECT_COLOR = Color{ 245, 245, 245, 155 }; // RAYWHITE but lower opacity

static inline Vector2 GetCamRectPos(int i) {
	return { CAM_RECT_SPACING * 2 + CAM_RECT_DIMS.x * i + CAM_RECT_SPACING * (i - 1),
			 GetScreenHeight() - CAM_RECT_DIMS.y - 5};
}
} // namespace UI

template <typename T>
static T Random(T min, T max) {
	return static_cast<T>(std::rand() % (max - min + 1)) + min;
}

static float RandomOffset(float offset) {
	return (std::rand() / float(RAND_MAX) * 2 - 1) * offset;
}

void NightMinigame::init() {
	// <random> header bloats code by a lot, assuming we want it low for web builds
	// so using c random
	std::srand(static_cast<unsigned>(std::time(NULL)));

	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Collision::HIT_CAMS, NightMinigame::StartMinigame)
	);

	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Energy::NO_ENERGY, NightMinigame::NoEnergy)
	);

	/*gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Time::DAY_BEGIN, NightMinigame::)
	);
	
	gCoordinator.AddEventListener(
		METHOD_LISTENER(Events::Time::NIGHT_BEGIN, NightMinigame::)
	);*/

	const Vector2 screenDims = { float(GetScreenWidth()), float(GetScreenHeight()) };

	// TODO: tweak me for design purposes :)
	// adjust these (> = farther, < = closer) to adjust where enemies spawn
	static constexpr int ZONE_CENTER = 111;
	static constexpr int SPAWN_OFFSET = 285;
	static constexpr int CAM_OFFSET_X = 60;
	static constexpr float BARRIER_OFFSET = 2.f;
	static constexpr Vector2 DMG_TOWER_POS = { ZONE_CENTER + 50, ZONE_CENTER + 30 };
	static constexpr Vector2 LIGHT_TOWER_POS = { ZONE_CENTER - 30, ZONE_CENTER - 30 };
	const std::array BARRIER_TOP_POS = { Vector2{ 0, m_base.height + BARRIER_OFFSET },
										 Vector2{ m_base.width + BARRIER_OFFSET, m_base.height + BARRIER_OFFSET } };
	const std::array BARRIER_SIDE_POS = { Vector2{ m_base.width + BARRIER_OFFSET, 0 },
										  Vector2{ m_base.width + BARRIER_OFFSET, m_base.height + BARRIER_OFFSET } };

	// enemies can spawn in a radius around two points for each camera. these are the points
	auto enemySpawnCenters = std::to_array<std::array<Vector2, 2>, 4>({
		// cam 1
		{ Vector2{ -SPAWN_OFFSET - CAM_OFFSET_X, -SPAWN_OFFSET / 2 }, Vector2{ -SPAWN_OFFSET / 2, -SPAWN_OFFSET } },
		// cam 2
		{ Vector2{  SPAWN_OFFSET + CAM_OFFSET_X, -SPAWN_OFFSET / 2 }, Vector2{  SPAWN_OFFSET / 2, -SPAWN_OFFSET } },
		// cam 3
		{ Vector2{ -SPAWN_OFFSET - CAM_OFFSET_X,  SPAWN_OFFSET / 2 }, Vector2{ -SPAWN_OFFSET / 2,  SPAWN_OFFSET } },
		// cam 4
		{ Vector2{  SPAWN_OFFSET + CAM_OFFSET_X,  SPAWN_OFFSET / 2 }, Vector2{  SPAWN_OFFSET / 2,  SPAWN_OFFSET } }
	});

	auto zoneCenters = std::to_array<Vector2, 4>({
		{ -ZONE_CENTER - CAM_OFFSET_X, -ZONE_CENTER },
		{  ZONE_CENTER + CAM_OFFSET_X, -ZONE_CENTER },
		{ -ZONE_CENTER - CAM_OFFSET_X,  ZONE_CENTER },
		{  ZONE_CENTER + CAM_OFFSET_X,  ZONE_CENTER }
	});

	// TODO: place defenses: DamageTower, LightTower, Barricade
	// create objects -- default initialization
	// tweak positions (defense.t2d.pos = xyz)
	// call m_zones[i].m_defenses.emplace_back(new DefenseType([default copy ctor]defense));
	// once per each defense wanted to add
	DamageTower dt;
	LightTower lt;
	Barricade top, side;

	auto dmgPos = std::to_array<Vector2, 4>({
		{ -DMG_TOWER_POS.x, -DMG_TOWER_POS.y },
		{  DMG_TOWER_POS.x, -DMG_TOWER_POS.y },
		{ -DMG_TOWER_POS.x,  DMG_TOWER_POS.y },
		{  DMG_TOWER_POS.x,  DMG_TOWER_POS.y }
	});

	auto lightPos = std::to_array<Vector2, 4>({
		{ -LIGHT_TOWER_POS.x, -LIGHT_TOWER_POS.y },
		{  LIGHT_TOWER_POS.x, -LIGHT_TOWER_POS.y },
		{ -LIGHT_TOWER_POS.x,  LIGHT_TOWER_POS.y },
		{  LIGHT_TOWER_POS.x,  LIGHT_TOWER_POS.y }
	});

	auto barrierTopPos = std::to_array<std::array<Vector2, 2>, 4>({
		{ BARRIER_TOP_POS[0] * -1, BARRIER_TOP_POS[1] * -1 },
		{ BARRIER_TOP_POS[0],	   BARRIER_TOP_POS[1] * -1 },
		{ BARRIER_TOP_POS[0] * -1, BARRIER_TOP_POS[1] },
		{ BARRIER_TOP_POS[0],	   BARRIER_TOP_POS[1] },
	});

	auto barrierSidePos = std::to_array<std::array<Vector2, 2>, 4>({
		{ BARRIER_SIDE_POS[0] * -1, BARRIER_SIDE_POS[1] * -1 },
		{ BARRIER_SIDE_POS[0],		BARRIER_SIDE_POS[1] * -1 },
		{ BARRIER_SIDE_POS[0] * -1, BARRIER_SIDE_POS[1] },
		{ BARRIER_SIDE_POS[0],		BARRIER_SIDE_POS[1] },
	});

	for (int i = 0; i < 4; ++i) {
		m_zones[i].m_pBase = &m_base;
		m_zones[i].m_center = zoneCenters[i];

		dt.t2d.pos = dmgPos[i];
		lt.t2d.pos = lightPos[i];
		
		top.t2d.pos = barrierTopPos[i][0];
		top.endPos = barrierTopPos[i][1];

		side.t2d.pos = barrierSidePos[i][0];
		side.endPos = barrierSidePos[i][1];

		m_zones[i].m_defenses.emplace_back(new DamageTower(dt));
		m_zones[i].m_defenses.emplace_back(new LightTower(lt));
		m_zones[i].m_defenses.emplace_back(new Barricade(top));
		m_zones[i].m_defenses.emplace_back(new Barricade(side));

		m_zones[i].init(enemySpawnCenters[i]);
	}

	m_zoneCam.offset = screenDims / 2;
	m_zoneCam.target = Vector2Zeros;
	m_zoneCam.zoom = 2.5f; // good enough :)
	m_curCam = 1;
}

void NightMinigame::shutdown() {
	for (auto& zone : m_zones) {
		zone.shutdown();
	}
}

void NightMinigame::HandleInput() {
	const Vector2 mousePos = GetMousePosition();
	const int key = GetKeyPressed();

	const float height = float(GetScreenHeight());
	for (int i = 0; i < 4; ++i) {
		Vector2 pos = UI::GetCamRectPos(i);
		Rectangle uiRect{ .x = pos.x, .y = pos.y,
						  .width = UI::CAM_RECT_DIMS.x, .height = UI::CAM_RECT_DIMS.y };

		if (CheckCollisionPointRec(mousePos, uiRect) &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			m_curCam = i + 1;
			break;
		}
	}

	if (key == KEY_ONE || key == KEY_TWO || key == KEY_THREE || key == KEY_FOUR) {
		m_curCam = key - KEY_ZERO;
	}
}

void NightMinigame::update(float dt) {
	HandleInput();

	m_zoneCam.target = m_zones[m_curCam - 1].m_center;

	m_base.hurtTimer -= dt;
	if (m_base.hurtTimer < 0)
		m_base.hurtTimer = 0;

	for (auto& zone : m_zones) {
		zone.update(dt);
	}

	// Success
	/*for (auto& zone : m_zones) {
		if (zone.m_enemiesToSpawn <= 0 && zone.m_enemiesInZone.empty() && m_base.hp.health > 0) {
			m_isResolved = true;
		}
	}*/

	// Defeat
	/*if (m_base.hp.health <= 0) {
		m_isResolved = true;
	}*/
}

// this turns on the night bool which determines whether or not we're actively
// updating this minigame, it should always be updating if it's night
void NightMinigame::StartMinigame(Event& e) {
	//m_isNight = true;
}

// this is to trakc which day we are on in the event we read in where / how
// many enemies are appearing based on the day
void NightMinigame::StartNewDay(Event& e) {
	static constexpr std::array ENEMY_SPAWNS_DAY = {
		15, // day 1
		22, // day 2
		35, // day 3
	};

	++m_currentDay;

	for (auto& zone : m_zones) {
		if (m_currentDay <= 3)
			zone.m_enemiesToSpawn = ENEMY_SPAWNS_DAY[m_currentDay];
		else
			zone.m_enemiesToSpawn = 0; // debug
	}

	Event scrChange{ Events::Game::SCREEN_CHANGE };
	scrChange.SetParam(Events::Game::SCREEN_ID, CAMERAS);
	gCoordinator.SendEvent(scrChange);
}

void NightMinigame::NoEnergy(Event& e) {
	for (auto& zone : m_zones) {
		for (auto& defense : zone.m_defenses) {
			defense->enabled = false;
		}
	}
}

void NightMinigame::combat_zone::init(const std::array<Vector2, 2>& enemySpawns) {
	m_spawner.init(enemySpawns);

	for (auto& defense : m_defenses) {
		defense->init();
	}
}

void NightMinigame::combat_zone::shutdown() {
	m_defenses.clear();
	m_enemiesInZone.clear(); // just in case
}

void NightMinigame::combat_zone::update(float dt) {
	if (m_enemiesToSpawn > 0)
		m_spawner.attempt_spawn(dt, m_enemySpawnRadius);

	UpdateEnemies(dt);
	
	for (auto& defense : m_defenses) {
		defense->update(dt, *this);
	}
}

void NightMinigame::enemy_spawner::init(std::array<Vector2, 2> enemySpawnCenterPoints, float spawnDelay) {
	m_EnemySpawnCenters = enemySpawnCenterPoints;
	m_spawnInterval = spawnDelay; // TODO: tweak me for design purposes :)
}

std::optional<NightMinigame::Enemy> NightMinigame::enemy_spawner::attempt_spawn(float dt, float spawnRadius) {
	m_spawnTimer -= dt;
	if (m_spawnTimer > 0)
		return std::nullopt;
	
	Vector2 spawnPoint = m_EnemySpawnCenters[Random(0, 1)];
	spawnPoint = Vector2Add(spawnPoint, { RandomOffset(spawnRadius), RandomOffset(spawnRadius) });

	m_spawnTimer = m_spawnInterval;
	return Enemy{ .t2d{ spawnPoint } };
}

void NightMinigame::combat_zone::UpdateEnemies(float dt) {
	if (m_enemiesToSpawn > 0) {
		auto enemyOpt = m_spawner.attempt_spawn(dt, m_enemySpawnRadius);

		if (enemyOpt.has_value()) {
			m_enemiesInZone.push_back(*enemyOpt);
			--m_enemiesToSpawn;
		}
	}

	for (auto it = m_enemiesInZone.begin(); it != m_enemiesInZone.end(); ) {
		Vector2 oldPos = it->t2d.pos;

		// Enemy is alive
		if (auto liveState = it->update(dt, m_baseBounds); liveState == Enemy::LivingState::ALIVE) {
			Vector2 baseCenter = { m_baseBounds.x + m_baseBounds.width / 2, m_baseBounds.y + m_baseBounds.height / 2 };
			Vector2 toBase = Vector2Subtract(baseCenter, it->t2d.pos);

			float dist = Vector2Length(toBase);

			// try damage base
			if (dist <= m_pBase->dmgRadius) {
				TryDamageBase(true);
				it->t2d.pos = oldPos; // move back
			}
			// get blocked by any barricades
			else {
				for (auto& defense : m_defenses) {
					if (Barricade* barricade = dynamic_cast<Barricade*>(defense.get())) {
						if (barricade->enabled && dist <= barricade->range) {
							TryDamageBase(false);
							it->t2d.pos = oldPos; // move back
							break;
						}
					}
				}
			}

			if (it->t2d.pos != oldPos) {
				// TODO: move fx
			}

			++it;
		}
		// Enemy has fleed / died
		else {
			// TODO: play enemy flee fx
			if (liveState == Enemy::LivingState::FLED) {

			}
			// TODO: play enemy die fx
			else {

			}

			it = m_enemiesInZone.erase(it);
		}
	}
}

void NightMinigame::combat_zone::TryDamageBase(bool healthDmg) {
	if (m_pBase->hurtTimer > 0)
		return;

	// TODO: send event? HEALTH_DMG?
	m_pBase->hurtTimer = m_pBase->hurtCooldown;

	if (healthDmg)
		gCoordinator.SendEvent(Events::Health::HEALTH_DMG);
	else {
		gCoordinator.SendEvent(Events::Energy::ENERGY_DOWN);
	}

	// TODO: base hit fx
}

NightMinigame::Enemy::LivingState NightMinigame::Enemy::update(float dt, const Rectangle& baseBounds) {
	if (hp.health <= 0) {
		// TODO: death fx
		return LivingState::DEAD;
	}

	return UpdateMovement(dt, baseBounds);
}

NightMinigame::Enemy::LivingState NightMinigame::Enemy::UpdateMovement(float dt, const Rectangle& baseBounds) {
	Vector2 baseCenter = { baseBounds.x + baseBounds.width / 2, baseBounds.y + baseBounds.height / 2 };
	
	if (state.tower.isFleeing) {
		Vector2 awayFromBase = Vector2Subtract(t2d.pos, baseCenter);

		Vector2 awayDir = Vector2Normalize(awayFromBase);

		t2d.pos = Vector2Add(t2d.pos, Vector2Scale(awayDir, state.speed * state.tower.fleeSpeedMult * dt));

		auto [x, y] = t2d.pos;
		Vector2 botRight = { baseBounds.x + baseBounds.width, baseBounds.y + baseBounds.height };
		if (x < baseBounds.x - ENEMY_LEAVE_BOUNDS || y < baseBounds.y - ENEMY_LEAVE_BOUNDS ||
			x > botRight.x + ENEMY_LEAVE_BOUNDS || y > botRight.y + ENEMY_LEAVE_BOUNDS)
		{
			return LivingState::FLED;
		}
	}
	else {
		Vector2 toBase = Vector2Subtract(baseCenter, t2d.pos);

		Vector2 toDir = Vector2Normalize(toBase);
		t2d.pos = Vector2Add(t2d.pos, Vector2Scale(toDir, state.speed * dt));
	}

	return LivingState::ALIVE;
}

void NightMinigame::Defense::update(float dt, combat_zone&) {
	if (enabled)
		TickEnergy(dt);
}

NightMinigame::Enemy* NightMinigame::Defense::GetClosestEnemyToBase(combat_zone& zone) {
	Vector2 baseCenter = { zone.m_baseBounds.x + zone.m_baseBounds.width / 2,
						   zone.m_baseBounds.y + zone.m_baseBounds.height / 2 };
	float closestDist = FLT_MAX;
	Enemy* closestEnemy = nullptr;

	for (Enemy& e : zone.m_enemiesInZone) {
		float dist = Vector2Distance(e.t2d.pos, baseCenter);

		if (dist < closestDist) {
			closestDist = dist;
			closestEnemy = &e;
		}
	}

	return closestEnemy;
}

void NightMinigame::Defense::TickEnergy(float dt) {
	timeUp += dt;

	if (dt >= energyTickTime) {
		timeUp = 0;
		
		Event tickEvent{ Events::Energy::ENERGY_DOWN };
		tickEvent.SetParam(Events::Energy::ENERGY_TICK, ENERGY_TICK_AMT);
		gCoordinator.SendEvent(tickEvent);
	}
}

void NightMinigame::DamageTower::update(float dt, combat_zone& zone) {
	if (!enabled)
		return;

	TickEnergy(dt);

	m_cooldownTimer -= dt;
	if (m_cooldownTimer <= 0) {
		m_cooldownTimer = 0;
		TryAttack(GetClosestEnemyToBase(zone));
	}
}

void NightMinigame::DamageTower::TryAttack(Enemy* closestEnemy) {
	if (closestEnemy == nullptr)
		return;

	float dist = Vector2DistanceSqr(closestEnemy->t2d.pos, t2d.pos);

	if (dist > range * range)
		return;

	// TODO: play tower shoot fx
	// TODO: play enemy dmg fx
	closestEnemy->hp.health -= dmg;

	m_cooldownTimer = cooldownTime;
}

void NightMinigame::LightTower::update(float dt, combat_zone& zone) {
	if (!enabled)
		return;

	TickEnergy(dt);

	for (Enemy& e : zone.m_enemiesInZone) {
		if (e.state.tower.isFleeing)
			continue;

		float dist = Vector2DistanceSqr(e.t2d.pos, t2d.pos);

		if (dist <= range * range) {
			// time that can be spent in light shortens
			e.state.tower.lightExposureTime += dt;

			// now enemy is afraid
			if (e.state.tower.lightExposureTime >= enemyFearTime) {
				e.state.tower.isFleeing = true;
			}
		}
		else {
			e.state.tower.lightExposureTime = 0;
		}
	}
}

void NightMinigame::draw() {
	/*if (!m_isVisible)
		return;*/

	BeginMode2D(m_zoneCam);
	
	DrawBase();
	m_zones[m_curCam - 1].draw();
	
	EndMode2D();

	DrawUI();
}

void NightMinigame::DrawBase() {
	static constexpr float INNER_SCALE = 0.8f;

	auto baseDimsOut = Rectangle{ .x = -m_base.width / 2.f, .y = -m_base.height / 2.f,
							   .width = float(m_base.width), .height = float(m_base.height) };
	auto baseDimsIn = Rectangle{ baseDimsOut.x * INNER_SCALE, baseDimsOut.y * INNER_SCALE,
								 baseDimsOut.width * INNER_SCALE, baseDimsOut.height * INNER_SCALE };
	DrawRectangleRounded(baseDimsOut, 1.f, 6, DARKGRAY); // dark gray
	DrawRectangleRounded(baseDimsIn, 1.f, 6, GRAY); // gray
}

void NightMinigame::DrawUI() {
	// camera text
	char camText[] = "Camera X";
	camText[7] = static_cast<char>('0' + m_curCam);

	DrawText(camText, UI::CAM_RECT_SPACING, UI::CAM_RECT_SPACING,
			 UI::FONT_SIZE, UI::CAM_RECT_COLOR);

	for (int i = 0; i < 4; ++i) {
		// camera switch ui boxe
		Vector2 pos = UI::GetCamRectPos(i);
		DrawRectangleV(pos, UI::CAM_RECT_DIMS, UI::CAM_RECT_COLOR);
		
		// number inside said boxes
		char num[2] = { char('0' + i + 1), '\0' };
		DrawText(num,
				 int(pos.x + UI::CAM_RECT_DIMS.x / 2) - UI::FONT_SIZE / 4,
				 int(pos.y + UI::CAM_RECT_DIMS.y / 2) - UI::FONT_SIZE / 2,
				 UI::FONT_SIZE, BLACK);
	}
}

void NightMinigame::combat_zone::draw() {
	for (auto& e : m_enemiesInZone) {
		e.draw();
	}

	for (auto& defense : m_defenses) {
		defense->draw();
	}
}

void NightMinigame::Enemy::draw() {
	DrawCircleV(t2d.pos, 3.f, Color{ 145, 10, 10, 255 }); // dark red
	DrawCircleV(t2d.pos, 1.5f, RED); // light red
}

static constexpr float DEFENSE_DRAW_SCALE = 5.f;
static constexpr Color DEFENSE_DISABLED_COLOR = LIGHTGRAY;

bool NightMinigame::DamageTower::CheckClicked() {

}

void NightMinigame::DamageTower::draw() {
	static constexpr float TRI_SCALE = 3.f * DEFENSE_DRAW_SCALE;

	Vector2 v1 = t2d.pos + Vector2{ 0, -TRI_SCALE / 2 };
	Vector2 v2 = t2d.pos + Vector2{ -TRI_SCALE / 2, TRI_SCALE / 2 };
	Vector2 v3 = t2d.pos + Vector2{ TRI_SCALE / 2, TRI_SCALE / 2 };

	// this should be in update but jankiness goes jank
	Vector2 mouse = GetMousePosition();
	if (CheckCollisionPointTriangle(mouse, v1, v2, v3) &&
		IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		enabled = !enabled;
	}

	DrawTriangle(v1, v2, v3, enabled ? SKYBLUE : DEFENSE_DISABLED_COLOR);
}

void NightMinigame::LightTower::draw() {
	//DrawCircleLines(t2d.pos,  MAGENTA);
	DrawRingLines(t2d.pos, 5 * DEFENSE_DRAW_SCALE, 10 * DEFENSE_DRAW_SCALE, 0, 360, 1,
				  enabled ? MAGENTA : DEFENSE_DISABLED_COLOR);
}

void NightMinigame::Barricade::draw() {
	// TODO: basic shapes -- barricades still have a bit of work, will finish later tonight or in the morning
	//DrawLineV() // light orange
	DrawLineV(t2d.pos, endPos, enabled ? ORANGE : DEFENSE_DISABLED_COLOR);
}
