#pragma once

#include <array>
#include <vector>
#include <optional>
#include <memory>

#include <raylib.h>

#include "Core.h"
// these were originally for if this was all gonna be ecs
// but since we're not doing that for the minigames,
// im just reusing what i already wrote
#include "Health.h"
#include "EnemyState.h"

class Event;

// spawning all the enemies
// managing the 4 combat zones

class NightMinigame {
	static constexpr int MAX_ZONES = 4;
	static constexpr int ENEMY_LEAVE_BOUNDS = 30;

public:
	void init();
	void update(float dt);
	void draw();
	void shutdown();
	
	inline bool active() const { return m_isNight; }
	
	// this turns on the drawing bool which determines whether
	// or not we're actively drawing this minigame on screen
	void ToggleStatus(Event& e);
	
	// this turns on the night bool which determines whether or not we're actively
	// updating this minigame, it should always be updating if it's night
	void StartMinigame(Event& e);

	// this is to track which day we are on in the event we read in where / how
	// many enemies are appearing based on the day
	void StartNewDay(Event& e);

// ---- Structs & Typedefs -----
private:
	struct combat_zone;

#pragma region Base Info
	struct Base {
		transform2D t2d;
		health hp = { 20 };
		float dmgRadius = 30.0f;
		float attackCooldown = 1.0f;

		float hurtTimer = 0;
		float hurtCooldown = 1.f;
	};
#pragma endregion Base Info
#pragma region Enemy
	struct Enemy {
		enum class LivingState : unsigned char {
			ALIVE,
			FLED,
			DEAD
		};

		transform2D t2d;
		health hp = { 5 };
		enemy_state state{ .speed = 10, .tower { .fleeSpeedMult = 1.25f } };
	
		// false = died / issue
		LivingState update(float dt, const Rectangle& baseBounds);
		void draw();

	private:
		LivingState UpdateMovement(float dt, const Rectangle& baseBounds);
	};

	class enemy_spawner {
	public:
		void init(std::array<Vector2, 2> enemySpawnCenterPoints, float spawnDelay);
		std::optional<Enemy> attempt_spawn(float dt, float spawnRadius);

	private:
		std::array<Vector2, 2> m_EnemySpawnCenters{};
		float m_spawnTimer = 0;

		float m_spawnInterval = 2;
	};
#pragma endregion Enemy
#pragma region Defenses
	struct Defense {
		transform2D t2d;
		float range;

		bool enabled = true; // TODO?

		virtual void init() {}
		virtual void update(float dt, combat_zone& zone) {}
		virtual void draw() = 0;

		virtual ~Defense() = default;

	protected:
		Enemy* GetClosestEnemyToBase(combat_zone& zone);
	};

	struct DamageTower final : Defense {
		float cooldownTime = 0.75f;
		int dmg = 1;

		void init() override { range = 250; }
		void update(float dt, combat_zone& zone) override;
		void draw() override;

	private:
		float m_cooldownTimer = 0;

		void TryAttack(Enemy* closestEnemy);
	};

	struct LightTower final : Defense {
		float enemyFearTime = 2.5f;

		void init() override { range = 200; }
		void update(float dt, combat_zone& zone) override;
		void draw() override;
	};

	struct Barricade final : Defense {
		void init() override { range = 60; }
		void draw() override;
	};
#pragma endregion Defenses

	struct combat_zone {
		// these were originally all private that's why they have m_
		std::vector<Enemy> m_enemiesInZone{};
		std::vector<std::unique_ptr<Defense>> m_defenses{};

		enemy_spawner m_spawner{};
		
		Base* m_pBase = nullptr;
		Rectangle m_bounds{}, m_baseBounds{};
		
		float m_enemySpawnRadius = 10;
		int m_enemiesToSpawn = 15; // TODO

		void init(const Rectangle& bounds, const Rectangle& baseBounds);
		void update(float dt);
		void draw();
		void shutdown();

	private:
		void UpdateEnemies(float dt);
		void TryDamageBase();
	};

// ----- Vars ----
private:
	std::array<combat_zone, MAX_ZONES> m_zones;
	Base m_base{};

	int m_currentDay = 0;
	bool m_isNight = false;		// update()
	bool m_isVisible = false;	// draw()

	bool m_isResolved = false;
};
