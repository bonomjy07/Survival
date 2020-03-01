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
public:
    std::vector<class MySprite*> _spriteList;
    
public:
    std::string getJSON_UnitsList() const;

public:
    /**
    @brief Gets information on spawnArea as ValueMap,  sprite to spawn and sprite image
     */
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

    /* Getter && Setter */
    uint32_t getMaxSpawnNumber() const;
    float getSpawnDelay() const;
    uint32_t getCurrentSpawnNumber() const;
    const std::string& getWhatToSpawn() const;
    
    void setSpawnDelay(float newSpawnDelay);
    void setMaxSpawnNumber(uint32_t newMaxSpawnNumber) const;
    void setWhatToSpawn(std::string newSpriteToSpawn);
    
private:
    /**
     @brief Information about spawnArea as ValueMap
     */
    cocos2d::ValueMap _spawnArea;
    
    /**
     @brief A string retains what to spawn a sprite
     */
    std::string _whatToSpawn;
    
    /**
     @brief A string retains sprite's image file name
     */
    std::string _filename;
    
    /**
     @brief Delay time for spawning an sprite
     */
    float _spawnDelay;
    
    /**
     @brief Number of maximum spawn sprite
     */
    uint32_t _maxSpawnNumber;
    
    /**
     @brief Number of currently spawned sprite
     */
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
    //cocos2d::Sprite* createSpriteToSpawn() const;
    class MySprite* createSpriteToSpawn() const;
    
};
#endif /* SpawnManager_h */
