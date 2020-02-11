//
//  SpawnManager.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/09.
//

#include "SpawnManager.h"
#include "TiledMapScene.h"
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
    _maxSpawnNumber = 20;
    _currentSpawnNumber = 0;
    _spawnDelay = 1.f;
}

void SpawnManager::startSpawn()
{
    SEL_SCHEDULE selector = schedule_selector(SpawnManager::spawnTheSprite);
    if (!this->isScheduled(selector))
    {
        this->schedule(selector, _spawnDelay);
    }
}

void SpawnManager::stopSpawn()
{
    SEL_SCHEDULE selector = schedule_selector(SpawnManager::spawnTheSprite);
    if (this->isScheduled(selector))
    {
        this->schedule(selector, _spawnDelay);
    }
}

void SpawnManager::spawnTheSprite(float dt) // dt is not used
{
    // Don't spawn if current spawned sprite is max
    if (_currentSpawnNumber >= _maxSpawnNumber) return;
    
    // Get director to tiledMapScene which is game-world
    if (auto director = Director::getInstance())
    {
        if (auto currentScene = director->getRunningScene())
        {
            // CurrentScene's layer is real game-playing layer
            auto gameLayer = currentScene->getChildByName("GameLayer");
            // Add new sprite in world(gameLayer)
            if (auto tiledMapScene = static_cast<TiledMapScene*>(gameLayer))
            {
                if (auto sprite = createSpriteToSpawn())
                {
                    sprite->setPosition(getRandomPointInArea());
                    tiledMapScene->addChild(sprite);
                    ++_currentSpawnNumber;
                    log("%s is spawned", _whatToSpawn.c_str());
                }
            }
        }
    }
    
}

Vec2 SpawnManager::getRandomPointInArea() const
{
    /**
     It doesn't retrun ".end()" at all, even if _spawnArea is not valid
     */
    // if (_spawnArea.find("x")     != _spawnArea.end() && _spawnArea.find("y")      != _spawnArea.end() &&
    //   _spawnArea.find("width") != _spawnArea.end() && _spawnArea.find("height") != _spawnArea.end())
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
    
    return Vec2::ZERO;
}

Sprite* SpawnManager::createSpriteToSpawn() const
{
    Sprite* sprite = nullptr;
    if (!_whatToSpawn.compare(""))
    {
        return sprite;
    }
    else if (!_whatToSpawn.compare("PawnSprite"))
    {
        sprite = PawnSprite::create(_filename, 100.f);
    }
    return sprite;
}
