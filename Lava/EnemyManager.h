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
    // enemy vars
    int enemyMaxHealth = 5;
    float enemyMoveSpeed = 10.0f;

    // base vars
    Vector2 baseLocation{};
    int baseMaxHealth = 20;
    float baseDamageRadius = 30.0f;
    float baseDamageCooldown = 1.0f;

    // spawning vars
    int enemiesToSpawn = 0;
    float spawnInterval = 1.0f;
    Vector2 spawnPointA{};
    Vector2 spawnPointB{};
    float spawnOffsetRadius = 10.0f;

    // runtime vars
    int baseCurrentHealth = baseMaxHealth;

    // enemy definition
    struct Enemy
    {
        Vector2 position;
        int health;
        float speed;

        //TODO - this is gross and ugly?
        Enemy(Vector2 pos, int hp, float spd)
            : position(pos), health(hp), speed(spd) {}
    };

    // internal vars
    std::vector<Enemy> enemies;
    float spawnTimer = 0.0f;
    float baseDamageTimer = 0.0f;
    bool isResolved = false;
#pragma endregion
    
    // CORE LOOP
    void Update(float deltaTime)
    {
        if (isResolved) return;

        UpdateSpawning(deltaTime);
        UpdateEnemies(deltaTime);
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
        //TODO - is this the right random?
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
        Vector2 toBase = Vector2Subtract(baseLocation, enemy.position);
        float distance = Vector2Length(toBase);

        //if in range, damage base and return
        if (distance <= baseDamageRadius)
        {
            TryDamageBase();
            return;
        }

        //else move towards the base
        Vector2 dir = Vector2Normalize(toBase);
        enemy.position = Vector2Add(enemy.position, Vector2Scale(dir, enemy.speed * deltaTime));

        PlayEnemyMoveFX(enemy.position);
    }

    // Base can only take damage X times per second
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
    
#pragma region EndConditions
    // check wincons
    void CheckEndConditions()
    {
        if (enemiesToSpawn <= 0 && enemies.empty() && baseCurrentHealth > 0)
        {
            Success();
        }
    }
    
    //TODO - What to do on win or lose?
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
    //TODO - FX?
    void PlayEnemySpawnFX(const Vector2&) {}
    void PlayEnemyMoveFX(const Vector2&) {}
    void PlayEnemyDeathFX(const Vector2&) {}
    void PlayBaseHitFX() {}
#pragma endregion

};
