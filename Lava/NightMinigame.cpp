#include "NightMinigame.h"

#include <raymath.h>
#include <ctime>
#include <cstdlib>
#include <cfloat>

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

	for (auto& zone : m_zones) {
		zone.m_pBase = &m_base;
	}
}

void NightMinigame::shutdown() {
	for (auto& zone : m_zones) {
		zone.shutdown();
	}
}

void NightMinigame::update(float dt) {
	if (!m_isNight || m_isResolved)
		return;

	m_base.hurtTimer -= dt;
	if (m_base.hurtTimer < 0)
		m_base.hurtTimer = 0;

	for (auto& zone : m_zones) {
		zone.update(dt);
	}

	// Success
	for (auto& zone : m_zones) {
		if (zone.m_enemiesToSpawn <= 0 && zone.m_enemiesInZone.empty() && m_base.hp.health > 0) {
			m_isResolved = true;
		}
	}

	// Defeat
	if (m_base.hp.health <= 0) {
		m_isResolved = true;
	}
}

// this turns on the night bool which determines whether or not we're actively
// updating this minigame, it should always be updating if it's night
void NightMinigame::StartMinigame(Event& e) {
	m_isNight = true;
}

// this turns on the drawing bool which determines whether
// or not we're actively drawing this minigame on screen
void NightMinigame::ToggleStatus(Event& e) {
	m_isVisible = !m_isVisible;
}

// this is to trakc which day we are on in the event we read in where / how
// many enemies are appearing based on the day
void NightMinigame::StartNewDay(Event& e) {
	++m_currentDay;
}

void NightMinigame::combat_zone::init(const Rectangle& bounds, const Rectangle& baseBounds) {
	m_bounds = bounds;
	m_baseBounds = baseBounds;
	// todo?
}

void NightMinigame::combat_zone::shutdown() {
	m_defenses.clear();
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
	m_spawnInterval = spawnDelay;
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
				TryDamageBase();
				it->t2d.pos = oldPos; // move back
			}
			// get blocked by any barricades
			else {
				for (auto& defense : m_defenses) {
					if (Barricade* barricade = dynamic_cast<Barricade*>(defense.get())) {
						if (barricade->enabled && dist <= barricade->range) {
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

void NightMinigame::combat_zone::TryDamageBase() {
	if (m_pBase->hurtTimer > 0)
		return;

	m_pBase->hp.health--;
	m_pBase->hurtTimer = m_pBase->hurtCooldown;

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

void NightMinigame::DamageTower::update(float dt, combat_zone& zone) {
	// TODO
	if (!enabled)
		return;

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
	if (!m_isVisible)
		return;

	// TODO: render system rework / integration? or manual drawing?

	for (auto& zone : m_zones) {
		zone.draw();
	}
}

void NightMinigame::combat_zone::draw() {
	// TODO
}

void NightMinigame::Enemy::draw() {
	// TODO
}

void NightMinigame::DamageTower::draw() {
	// TODO
}

void NightMinigame::LightTower::draw() {
	// TODO
}

void NightMinigame::Barricade::draw() {
	// TODO
}
