#pragma once
#include <vector>
#include <cstdlib>
#include "raylib.h"
#include "raymath.h"

class EnemyManager
{
public:
    static constexpr int MAX_ZONES = 4;

    //TODO - Connect this to the code below?--------------------V
    CombatZone zones[MAX_ZONES];                                //
                                                                //
    void Update(float deltaTime)                                //
    {                                                           //
        for (int i = 0; i < MAX_ZONES; ++i)                     //
        {                                                       //
            zones[i].Update(deltaTime);                         //
        }                                                       //
    }                                                           //
};                                                              //
                                                                //
//TODO - Can i do this?  :)      <--------------------------------
struct CombatZone
{
#pragma region CombatVariables
    
    //TODO - Connect health via another script
    // base vars
    Vector2 baseLocation{};
    int baseMaxHealth = 20;
    float baseDamageRadius = 30.0f;
    float baseDamageCooldown = 1.0f;
    
    // enemy vars
    int enemyMaxHealth = 5;
    float enemyMoveSpeed = 10.0f;

    // spawning vars
    int enemiesToSpawn = 0;
    float spawnInterval = 1.0f;
    Vector2 spawnPointA{};
    Vector2 spawnPointB{};
    float spawnOffsetRadius = 10.0f;
    
    // shared tower tuning
    float towerAttackRange = 250.0f;
    float towerAttackCooldown = 0.75f;
    int towerDamage = 1;
    float towerATimer = 0.0f;
    float towerBTimer = 0.0f;
    
    //TODO - tower toggles (another script controls these)
    bool towerAEnabled = true;
    bool towerBEnabled = true;
    bool lightTowerEnabled = true;
    bool barricadeEnabled = true;

    // light tower vars
    Vector2 lightTowerPosition{};
    float lightTowerRange = 200.0f;
    float lightTowerFearTime = 2.5f;
    float fleeSpeedMultiplier = 1.75f;

    // barricade tuning
    float barricadeRadius = 60.0f;
    
    // enemy definition
    struct Enemy
    {
        Vector2 position;
        int health;
        float speed;

        // Light tower state
        bool isFleeing = false;
        float lightExposureTimer = 0.0f;

        Enemy(Vector2 pos, int hp, float spd)
            : position(pos), health(hp), speed(spd) {}
    };
    
    // internal vars
    std::vector<Enemy> enemies;
    float spawnTimer = 0.0f;
    float baseDamageTimer = 0.0f;
    int baseCurrentHealth = baseMaxHealth;
    bool isResolved = false;
    
#pragma endregion
    
    // CORE LOOP
    void Update(float deltaTime)
    {
        if (isResolved) return;

        UpdateSpawning(deltaTime);
        UpdateEnemies(deltaTime);
        UpdateLightTower(deltaTime);
        UpdateTowers(deltaTime);
        CheckEndConditions();
    }

    //TODO - Private here?
private:
#pragma region EnemyAssaultLoop
    // Spawning 
    void UpdateSpawning(float deltaTime)
    {
        //check timer
        if (enemiesToSpawn <= 0) return;

        spawnTimer -= deltaTime;
        if (spawnTimer > 0.0f) return;

        //rand spawn
        //TODO - is this the right random to use?
        Vector2 spawnPoint = (std::rand() % 2 == 0) ? spawnPointA : spawnPointB;

        float offsetX = ((std::rand() / (float)RAND_MAX) * 2 - 1) * spawnOffsetRadius;
        float offsetY = ((std::rand() / (float)RAND_MAX) * 2 - 1) * spawnOffsetRadius;

        spawnPoint.x += offsetX;
        spawnPoint.y += offsetY;

        //instanciate, play FX, reset timer, and reduce enemiesToSpawn
        enemies.emplace_back(spawnPoint, enemyMaxHealth, enemyMoveSpeed);
        enemiesToSpawn--;

        PlayEnemySpawnFX(spawnPoint);
        spawnTimer = spawnInterval;
    }

    // Enemies
    void UpdateEnemies(float deltaTime)
    {
        //update timer
        baseDamageTimer -= deltaTime;

        //for each enemy check health then update its movement
        for (int i = (int)enemies.size() - 1; i >= 0; --i)
        {
            Enemy& enemy = enemies[i];

            if (enemy.health <= 0)
            {
                PlayEnemyDeathFX(enemy.position);
                enemies.erase(enemies.begin() + i);
                continue;
            }

            UpdateEnemyMovement(enemy, deltaTime);
        }
    }

    // movement
    void UpdateEnemyMovement(Enemy& enemy, float deltaTime)
    {
        //Check if fleeingm and make the enemy move away
        if (enemy.isFleeing)
        {
            Vector2 awayFromLight = Vector2Subtract(enemy.position, lightTowerPosition);
            Vector2 dir = Vector2Normalize(awayFromLight);

            enemy.position = Vector2Add(
                enemy.position,
                Vector2Scale(dir, enemy.speed * fleeSpeedMultiplier * deltaTime)
            );

            // off-screen cleanup of enemies
            //TODO - Check if this works?
            if (enemy.position.x < -50 || enemy.position.x > (float)GetScreenWidth() + 50 ||
                enemy.position.y < -50 || enemy.position.y > (float)GetScreenHeight() + 50)
            {
                //reuses death cleanup
                PlayEnemyFledFX(enemy.position);
                enemy.health = 0;
            }

            return;
        }

        // Calculate distances
        Vector2 toBase = Vector2Subtract(baseLocation, enemy.position);
        float distance = Vector2Length(toBase);

        // If Barricade check (circle around base, larger than damage radius)
        if (barricadeEnabled && distance <= barricadeRadius && distance > baseDamageRadius)
        {
            return; // Full stop when touching the barricade
        }

        // else If Damage check
        if (distance <= baseDamageRadius)
        {
            TryDamageBase();
            return;
        }

        // Else Move towards base
        Vector2 dir = Vector2Normalize(toBase);
        enemy.position = Vector2Add(enemy.position, Vector2Scale(dir, enemy.speed * deltaTime));

        PlayEnemyMoveFX(enemy.position);
    }

    // Base can only take damage X times per second
    //TODO - Connect to actual gameplay for base health
    void TryDamageBase()
    {
        if (baseDamageTimer > 0.0f) return;

        baseCurrentHealth--;
        baseDamageTimer = baseDamageCooldown;

        PlayBaseHitFX();

        if (baseCurrentHealth <= 0)
        {
            baseCurrentHealth = 0;
            Defeat();
        }
    }
    
#pragma endregion

#pragma region TowerCombat
    // Tower timer update loop
    void UpdateTowers(float deltaTime)
    {
        if (enemies.empty()) return;
        
        towerATimer -= deltaTime;
        towerBTimer -= deltaTime;

        if (towerAEnabled && towerATimer <= 0.0f)
        {
            TryTowerAttack(towerATimer);
        }

        if (towerBEnabled && towerBTimer <= 0.0f)
        {
            TryTowerAttack(towerBTimer);
        }
    }

    // Tower attacks deal damage to enemy
    void TryTowerAttack(float& towerTimer)
    {
        //get enemy in range, check our range, then damage and reset timer
        Enemy* target = GetEnemyClosestToBase();
        if (!target) return;

        float distanceToBase = Vector2Distance(target->position, baseLocation);
        if (distanceToBase > towerAttackRange) return;

        target->health -= towerDamage;

        PlayTowerAttackFX(target->position);

        towerTimer = towerAttackCooldown;
    }

    // Fetch enemy closest to base
    Enemy* GetEnemyClosestToBase()
    {
        //TODO - check C++ values work
        Enemy* closestEnemy = nullptr; //google said this :)
        float closestDist = FLT_MAX; //google said this :)

        for (Enemy& enemy : enemies)
        {
            float dist = Vector2Distance(enemy.position, baseLocation);
            if (dist < closestDist)
            {
                closestDist = dist;
                closestEnemy = &enemy;
            }
        }

        return closestEnemy;
    }
    
    // Light tower causes fleeing
    void UpdateLightTower(float deltaTime)
    {
        if (!lightTowerEnabled || enemies.empty()) return;

        //for each enemy, check range, add to their timer, toggle fleeing if exposed
        for (int i = (int)enemies.size() - 1; i >= 0; --i)
        {
            Enemy& enemy = enemies[i];

            if (enemy.isFleeing) continue;

            float dist = Vector2Distance(enemy.position, lightTowerPosition);
            if (dist <= lightTowerRange)
            {
                enemy.lightExposureTimer += deltaTime;

                if (enemy.lightExposureTimer >= lightTowerFearTime)
                {
                    enemy.isFleeing = true;
                    PlayEnemyFleeFX(enemy.position);
                }
            }
            else
            {
                enemy.lightExposureTimer = 0.0f;
            }
        }
    }
    
#pragma endregion
    
#pragma region EndConditions
    // check wincons
    void CheckEndConditions()
    {
        if (enemiesToSpawn <= 0 && enemies.empty() && baseCurrentHealth > 0)
        {
            Success();
        }
    }
    
    //TODO - TEMP please connect to actual game
    void Success()
    {
        isResolved = true;
        Cleanup();
    }

    void Defeat()
    {
        isResolved = true;
        Cleanup();
    }

    void Cleanup()
    {
        enemies.clear();
    }
    
#pragma endregion

#pragma region FX
    
    // FX
    //TODO - Please connect to actual audio and vfx calls
    void PlayEnemySpawnFX(const Vector2&) {}
    void PlayEnemyMoveFX(const Vector2&) {}
    void PlayEnemyDeathFX(const Vector2&) {}
    void PlayBaseHitFX() {}
    void PlayTowerAttackFX(const Vector2&) {}
    void PlayEnemyFleeFX(const Vector2&) {}
    void PlayEnemyFledFX(const Vector2&) {}

#pragma endregion

};
