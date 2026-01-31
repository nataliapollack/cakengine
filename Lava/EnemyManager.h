#pragma once
#include <vector>
#include <cstdlib>
#include "raylib.h"


//TODO - Swap name to NightManager
class EnemyManager
{
public:
    //ENEMY VARS
    int enemyMaxHealth = 5;
    float enemyMoveSpeed = 10.0f;
    
    //BASE VARS
    Vector2 baseLocation;
    int baseMaxHealth = 20;
    float baseDamageRadius = 3.0f;
    float baseDamageCooldown = 1.0f;

    //SPAWNING VARS
    int enemiesToSpawn = 0;
    float spawnInterval = 1.0f;
    Vector2 spawnPointA;
    Vector2 spawnPointB;
    float spawnOffsetRadius = 1.0f;
    
    //Runtime
    int baseCurrentHealth = 20;
   
    //Enemy goes here??????
    struct Enemy
    {
        Vector2 position;
        int health = 5;
        float speed = 10.0f;

        //THIS IS GROSS
        Enemy(const Vector2& spawnPos, int hp, float spd)
            : position(spawnPos), health(hp), speed(spd)
        {}
    };
    
    //------------------------
    
    //TODO - How to connect this to game?
    //TODO - Make this work for four instances / corners
    void Update(float deltaTime)
    {
        UpdateSpawning(deltaTime);
        UpdateEnemies(deltaTime);
        CheckEndConditions();
    }

private:
    //inside script code?
    std::vector<Enemy> enemies;

    float spawnTimer = 0.0f;
    float baseDamageTimer = 0.0f;

    //Spawn Enemy Loop
    void UpdateSpawning(float deltaTime)
    {
        //Check valid enemies & valid time
        if (enemiesToSpawn <= 0) return;
        spawnTimer -= deltaTime;
        if (spawnTimer > 0.0f) return;

        //TODO - RANDOM????
        //Pick spawn point
        Vector2 spawnPoint = (std::rand() % 2 == 0) ? spawnPointA : spawnPointB;

        //Create and add offset
        float offsetX = ((std::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnOffsetRadius;
        float offsetY = ((std::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * spawnOffsetRadius;

        spawnPoint.x += offsetX;
        spawnPoint.y += offsetY;

        //Instance 
        enemies.emplace_back(spawnPoint, enemyMaxHealth, enemyMoveSpeed);
        enemiesToSpawn--;

        //Play FX and reset timer
        PlayEnemySpawnFX(spawnPoint);

        spawnTimer = spawnInterval;
    }
    
    //Enemy Update Loop
    void UpdateEnemies(float deltaTime)
    {
        baseDamageTimer -= deltaTime;

        //THIS IS GROSS
        //For each enemy, check its health
        for (int i = static_cast<int>(enemies.size()) - 1; i >= 0; --i)
        {
            Enemy* enemy = enemies[i];

            if (enemy->health <= 0)
            {
                PlayEnemyDeathFX(enemy->position);
                
                //TODO - Is this correct?
                delete enemy;
                enemies.erase(enemies.begin() + i);
                continue;
            }

            UpdateEnemyMovement(enemy, deltaTime);
        }
    }

    //Enemy movement loop per enemy
    void UpdateEnemyMovement(Enemy* enemy, float deltaTime)
    {
        //TODO - dot product here instead of length?
        Vector2 toBase = baseLocation - enemy->position;
        float distance = toBase.Length();

        //If we are inside the radius/at the base then we try damage
        if (distance <= baseDamageRadius)
        {
            TryDamageBase();
            return;
        }

        //TODO - Normalize?
        Vector2 direction = toBase.Normalized();
        enemy->position = enemy->position + direction * enemy->speed * deltaTime;

        PlayEnemyMoveFX(enemy->position);
    }

    // Base can only take damage X times per second
    void TryDamageBase()
    {
        //Check against timer
        if (baseDamageTimer > 0.0f) return;

        //Damage the base and start the damage timer again
        baseCurrentHealth--;
        baseDamageTimer = baseDamageCooldown;

        PlayBaseHitFX();

        //If base is 0 hp then play defete
        if (baseCurrentHealth <= 0)
        {
            baseCurrentHealth = 0;
            Defeat();
        }
    }

    //Check for wincon
    void CheckEndConditions()
    {
        if (enemiesToSpawn <= 0 && enemies.empty() && baseCurrentHealth > 0)
        {
            Success();
        }
    }
    
    //Towers damage selected enemy
    void DamageEnemy(Enemy* enemy)
    {
        if (!enemy || enemy->health <= 0) return;

        enemy->health--;

        PlayEnemyHitFX(enemy->position);
    }
    
    //----------------------------------------------

    //FX
    void PlayEnemySpawnFX(const Vector2& pos) {}
    void PlayEnemyMoveFX(const Vector2& pos) {}
    void PlayEnemyHitFX(const Vector2& pos) {}
    void PlayEnemyDeathFX(const Vector2& pos) {}
    void PlayBaseHitFX() {}

    void Success()
    {
        // Victory VFX / Audio
        Cleanup();
    }

    void Defeat()
    {
        // Failure VFX / Audio
        Cleanup();
    }

    void Cleanup()
    {
        for (Enemy* enemy : enemies)
            delete enemy;

        enemies.clear();
    }
};
