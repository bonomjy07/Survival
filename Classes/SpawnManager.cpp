//
//  SpawnManager.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/09.
//

#include "SpawnManager.h"
#include "TestScene.h"
#include "MySprite.h"
#include "UnitSprite.h"
#include "PawnSprite.h"

USING_NS_CC;

SpawnManager *SpawnManager::create(const ValueMap& spawnArea,
                                   const std::string& whatToSpawn,
                                   const std::string& filename)
{
    SpawnManager* manager = new (std::nothrow) SpawnManager(spawnArea, whatToSpawn, filename);
    if (manager)
    {
        manager->autorelease();
        return manager;
    }
    
    CC_SAFE_DELETE(manager);
    return nullptr;
}

SpawnManager::SpawnManager(const cocos2d::ValueMap &spawnArea,
                           const std::string &whatToSpawn,
                           const std::string &filename)
                           : _spawnArea(spawnArea), _whatToSpawn(whatToSpawn), _filename(filename)
{
    log("Spawn Manager for %s created", whatToSpawn.c_str());
    _maxSpawnNumber = 10;
    _currentSpawnNumber = 0;
    _spawnDelay = 0.1f;
}

std::string SpawnManager::getJSON_UnitsList() const
{
    std::string JSONList = "[";
    for (const auto sprite : _spriteList)
    {
        std::string ID, x, y, data;
        ID = ID+"\"ID\":"+"\""+sprite->getName()+"\"";
        x = x+"\"x\":"+"\""+std::to_string(sprite->getPositionX())+"\"";
        y = y+"\"y\":"+"\""+std::to_string(sprite->getPositionY())+"\"";
        data = "{"+ID+","+x+","+y+"}";
        JSONList += data + ",";
    }
    JSONList.erase(JSONList.length()-1);
    JSONList += "]";
    
    return JSONList;
}

void SpawnManager::startSpawn()
{
    SEL_SCHEDULE selector = schedule_selector(SpawnManager::spawnTheSprite);
    if (!this->isScheduled(selector))
    {
        this->schedule(selector, _spawnDelay);
        log("Spawning %s is started", _whatToSpawn.c_str());
    }
}

void SpawnManager::stopSpawn()
{
    SEL_SCHEDULE selector = schedule_selector(SpawnManager::spawnTheSprite);
    if (this->isScheduled(selector))
    {
        this->unschedule(selector);
        log("Spawning %s is stopped", _whatToSpawn.c_str());
    }
}

void SpawnManager::spawnTheSprite(float dt) // dt is not used
{
    if (_currentSpawnNumber >= _maxSpawnNumber)
    {
        stopSpawn();
        return;
    }
    
    // 1. SpawnManager needs gameLayer
    if (auto gameLayer = dynamic_cast<GameLayer*>(this->_parent))
    {
        // 2. Get valid random poisition
        auto randomPosition = getRandomPointInArea();
        if (!gameLayer->checkNodeAtPosition(randomPosition))
        {
            if (auto sprite = createSpriteToSpawn())
            {
                sprite->setPosition(randomPosition);
                gameLayer->addChild(sprite);
                ++_currentSpawnNumber;
            }
        }
    }
}

Vec2 SpawnManager::getRandomPointInArea() const
{
    float x = _spawnArea.at("x").asFloat();
    float y = _spawnArea.at("y").asFloat();
    float width = _spawnArea.at("width").asFloat();
    float height = _spawnArea.at("height").asFloat();
    
    float randX = floor(RandomHelper::random_real(x, x+width));
    float randY = floor(RandomHelper::random_real(y, y+height));
    // Trim random position as tile position
    randX = randX - ((int)randX % 32) + 32/2;
    randY = randY - ((int)randY % 32) + 32/2;
    return Vec2(randX, randY);
}

MySprite* SpawnManager::createSpriteToSpawn() const
{
    MySprite* sprite = nullptr;
    if (!_whatToSpawn.compare(""))
    {
        return sprite;
    }
    else if (!_whatToSpawn.compare("PawnSprite"))
    {
        sprite = PawnSprite::create(_filename, 100.f);
    }
    else if (!_whatToSpawn.compare("UnitSprite"))
    {
        sprite = UnitSprite::create(_filename);
    }
    return sprite;
}
