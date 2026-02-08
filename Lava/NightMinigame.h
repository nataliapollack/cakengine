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

	// TODO: tweak me for design purposes :)
	static constexpr int ENEMY_LEAVE_BOUNDS = 500; // how far off screen enemy needs to be to despawn
	
	// TODO: tweak me for design purposes :)
	static constexpr int ENERGY_TICK_AMT = 1; // when energy event gets sent out

public:
	void init();
	void update(float dt);
	void draw();
	void shutdown();
	
	// TODO
	inline bool active() const { return true; }//m_isNight; }
	
	// this turns on the night bool which determines whether or not we're actively
	// updating this minigame, it should always be updating if it's night
	void StartMinigame(Event& e);

	// this is to track which day we are on in the event we read in where / how
	// many enemies are appearing based on the day
	void StartNewDay(Event& e);

	void NoEnergy(Event& e);

// ----- Funcs -----
private:
	void HandleInput();

	void DrawUI();
	void DrawBase();

// ----- Structs & Typedefs -----
/*
imma be real, if you need to refactor this significantly, just get rid of
all the struct definitions, and just copy-paste the variables and functions
to be in the main class, duplicate/rename as needed. probably won't need
to do this unless it becomes a pain to pass certain info around but eh
*/
private:
	struct combat_zone;

#pragma region Base Info
	struct Base {
		transform2D t2d;
		int width = 100;
		int height = 75;

		float dmgRadius = 10.0f; // TODO: tweak me for design purposes :)
		//float attackCooldown = 1.0f; // TODO: tweak me for design purposes :)

		float hurtTimer = 0;
		float hurtCooldown = 1.f; // TODO: tweak me for design purposes :)
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
		health hp = { 5 }; // TODO: tweak me for design purposes :)
		enemy_state state{ .speed = 12, .tower { .fleeSpeedMult = 1.75f } }; // TODO: tweak just the listed vars for design purposes :)
	
		// false = died / issue
		LivingState update(float dt, const Rectangle& baseBounds);
		void draw();

	private:
		LivingState UpdateMovement(float dt, const Rectangle& baseBounds);
	};

	class enemy_spawner {
	public:
		void init(std::array<Vector2, 2> enemySpawnCenterPoints, float spawnDelay = 0.1f);// TODO: tweak me for design purposes :)
		std::optional<Enemy> attempt_spawn(float dt, float spawnRadius);

	private:
		std::array<Vector2, 2> m_EnemySpawnCenters{};
		float m_spawnTimer = 0;

		float m_spawnInterval;
	};
#pragma endregion Enemy
#pragma region Defenses
	struct Defense {
		transform2D t2d = { .pos = {0, 0} }; // TODO: need to place defenses and manually set their positions
		float range = 1;
		int energyTick = 1;

		bool enabled = true; // TODO?

		virtual inline void init() {}
		virtual void update(float dt, combat_zone&);
		virtual void draw() = 0;

		virtual bool CheckClicked(const Camera2D& cam) = 0;

		virtual ~Defense() = default;

	protected:
		float timeUp = 0;
		float energyTickTime = 3.f; // TODO: tweak me for design purposes :)

		Enemy* GetClosestEnemyToBase(combat_zone& zone);
		void TickEnergy(float dt);
	};

	struct DamageTower final : Defense {
		float cooldownTime = 0.75f; // TODO: tweak me for design purposes :)
		int dmg = 1; // TODO: tweak me for design purposes :)

		// just keep range large so it affects all enemies in zone
		inline void init() override { range = 500; energyTick = 2; }
		void update(float dt, combat_zone& zone) override;
		void draw() override;

		bool CheckClicked(const Camera2D& cam) override;

	private:
		float m_cooldownTimer = 0;

		void TryAttack(Enemy* closestEnemy);
	};

	struct LightTower final : Defense {
		float enemyFearTime = 17.5f; // TODO: tweak me for design purposes :)

		inline void init() override { range = 500; } // just keep range large so it affects all enemies in zone
		void update(float dt, combat_zone& zone) override;
		void draw() override;

		bool CheckClicked(const Camera2D& cam) override;
	};

	struct Barricade final : Defense {
		Vector2 endPos{};

		inline void init() override { range = 5; energyTick = 3; }
		void draw() override;

		bool CheckClicked(const Camera2D& cam) override;
	};
#pragma endregion Defenses

	struct combat_zone {
		// these were originally all private that's why they have m_
		std::vector<Enemy> m_enemiesInZone{};
		std::vector<std::unique_ptr<Defense>> m_defenses{};

		enemy_spawner m_spawner{};
		
		Base* m_pBase = nullptr;
		Rectangle m_baseBounds{ 0, 0, 0, 0 }; // center of map basically -- this should be reworked maybe probably not it works its fine :)
		
		Vector2 m_center{};

		float m_enemySpawnRadius = 30; // TODO: tweak me for design purposes :)
		int m_enemiesToSpawn = 50; // TODO: tweak me for design purposes :)

		void init(const std::array<Vector2, 2>& enemySpawns);
		void update(float dt);
		void draw();
		void shutdown();

	private:
		void UpdateEnemies(float dt);
		void TryDamageBase(bool healthDmg);
	};

// ----- Vars ----
private:
	std::array<combat_zone, MAX_ZONES> m_zones;
	Base m_base{};

	Camera2D m_zoneCam{ 0 };
	int m_curCam = 1;

	int m_currentDay = 0;
	//bool m_isNight = false;		// update()
	//bool m_isVisible = false;	// draw()

	//bool m_isResolved = false;
};
