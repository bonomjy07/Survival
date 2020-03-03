//
//  SpawnManager.h
//  Survival
//
//  Created by jy_maeng on 2020/02/09.
//

#pragma once

#ifndef SpawnManager_h
#define SpawnManager_h

#include "cocos2d.h"

class SpawnManager : public cocos2d::Node
{
public:
    static SpawnManager* create(const cocos2d::ValueMap& spawnArea,
                                const std::string& whatToSpawn,
                                const std::string& filename
                                );
private:
    /* Vector that stores spawned sprite */
    cocos2d::Vector<class MySprite*> _spawnedList;
    
    /* Spawned sprite ID which'll increment whenvere a sprtie is spawned */
    int _spawnedID;

public:
    /* Set base information where and what to spawn */
    SpawnManager(const cocos2d::ValueMap& spawnArea, const std::string& whatToSpawn, const std::string& filename);
    
    /**
     @brief Start to spawn a sprite untill stopSpawn is called or spawned sprite is maximum.
     Called by TiledMapScene
     */
    void startSpawn();
    /**
     @brief Stop to spawn a sprite. Called when spawned sprite is maximum
     Called by TiledMapScene
     */
    void stopSpawn();
    
    /**
     @brief Called per spawnDelay and spawn a sprite
     */
    void spawnTheSprite(float dt);
    
    /* Accessor function for vector which stores spawned sprite */
    cocos2d::Vector<class MySprite*> getSpawnedList() const;
    
    /* Accessor function for max spawn number */
    uint32_t getMaxSpawnNumber() const;
    
    /* Accessor function for spawning delay */
    float getSpawnDelay() const;
    
    /* Accessor function for number of current spawned sprite */
    uint32_t getCurrentSpawnNumber() const;
    
    /* Accessor function for what to spawn  */
    const std::string& getWhatToSpawn() const;
    
private:
    /* Information about spawn area */
    cocos2d::ValueMap _spawnArea;
    
    /* The sprite to spawn */
    std::string _whatToSpawn;
    
    /* Texture of sprite to spawn */
    std::string _filename;
    
    /* Delay time for spawning an sprite */
    float _spawnDelay;
    
    /* Number of maximum spawning */
    uint32_t _maxSpawnNumber;
    /* Number of currently spawned sprite */
    uint32_t _currentSpawnNumber;
    
private:
    /**
     @brief Retunrs random point in area.
     If spawnArea is something wrong, returns Vec2::ZERO
     */
    cocos2d::Vec2 getRandomPointInArea() const;
    
    /**
     @brief Returns a sprite on whatToSpawn.
     If whatToSpawn is not class, returns nullptr.
     */
    class MySprite* createSpriteToSpawn() const;
    
};
#endif /* SpawnManager_h */
