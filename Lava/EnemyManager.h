#pragma once

class EnemyManager
{
public:
    /*
    [Header("Enemy Vars")]
    public sprite enemySprite;
    public int enemyHealth = 5;
    public float enemySpeed = 10f;

    [Header("Base Vars")]
    public vector2 targetLocation;
    public float damageDistance = 3;
    public int baseHealth = 20;
    
    [Header("Spawning Vars")]
    public int enemyToSpawnCount;
    private float spawnDelay;
    public float spawnTimer
    public vector2 spawnA;
    public vector2 spawnB;
    private gameObject[] enemyList;
    
    
    //Call this to play the minigame
    public void EnemyGameplay()
    {
        if(enemiesToSpawnCount > 0)
            EnemySpawner()
            UpdateEnemyList()
        else
            Success()
    }
    
    
    private void EnemySpawner
    {
        if(spawnDelay > 0)
            --spawnDelay vs delta.time
        else
            instantiate enemy prefab and add it to the enemyList[]
            
            //Play audio queue for spawning an enemy
            
            //reset spawn delay to spawnTimer so we have consistent delays between spawns
            spawnDelay = spawnTimer;
    }


    private void UpdateEnemyList
    {
        for enemyList
            if (enemyHealth == 0)
                removeFromList.enemyList;
        else 
            call updateEnemyPosition()
    }


    private void UpdateEnemyPosition()
    {
        myTransform = current position
        vector math to get position vs targetLocation
        
        if (distance <= damageDistance)
            //dont move, instead deal damage
            DamageBase();
        else
            move at enemySpeed towards the target
            //enemy shuffling or move audio?
    }


    public void DamageEnemy(gameobject targetEnemy)
    {
        --targetEnemy.enemyHealth;
        
        //Any damage visuals or audio
    }


    private void DamageBase()
    {
        //damage delay timer
        if(damageDelay > 0)
            --damageDelay vs delta.time
        else
            --baseHealth;
        
            //Play any damage effect or audio
    }
    
    
    public void Success()
    {
        //Play audio and vfx, then swap to other scene
    }
    
    
     */
    
};
