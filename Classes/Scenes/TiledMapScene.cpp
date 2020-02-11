//
//  TiledMapScene.cpp
//  MyGame
//
//  Created by jy_maeng on 2020/01/31.
//

#include "TiledMapScene.h"
#include "PawnSprite.h"
#include "KeyTableScene.h"
#include "SpawnManager.h"

#include <string>

USING_NS_CC;

Scene* TiledMapScene::createScene()
{
    auto scene = Scene::create();
    
    if (scene)
    {
        scene->initWithPhysics();
        auto world = scene->getPhysicsWorld();
        world->setAutoStep(false);
    }
    
    TiledMapScene* layer = TiledMapScene::create();
    layer->setName("GameLayer");
    scene->addChild(layer);
    
    return scene;
}

bool TiledMapScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // Create tile map and layer in tile map
    _tileMap = TMXTiledMap::create("res/TileGameResources/TileMap.tmx");
    _meta = _tileMap->getLayer("Meta");
    this->addChild(_tileMap);
    
    // Get SpawnPoint location
    TMXObjectGroup* objectGroup = _tileMap->getObjectGroup("Objects");
    if (objectGroup == NULL)
    {
        log("No object group naemd : Objects");
        return false;
    }
    ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();
    
    // Create spawn manager for tree
    ValueMap spawnArea = objectGroup->getObject("SpawnArea");
    SpawnManager* pawnManager = SpawnManager::create(spawnArea, "PawnSprite", "res/TileGameResources/Player.png");
    if (pawnManager)
    {
        this->addChild(pawnManager);
        pawnManager->startSpawn();
    }
    
    // Create player character
    _player = PawnSprite::create("res/TileGameResources/Player.png", 100.f);
    if (_player)
    {
        _player->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(_player);
        this->setViewPointCenter(_player->getPosition());
        _player->initPhysics();
    }
    
    // Register keyboard listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TiledMapScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(TiledMapScene::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Allow update(float dt) to be called so that pawns move
    this->scheduleUpdate();
    
    return true;
}

void TiledMapScene::setViewPointCenter(const cocos2d::Vec2 position)
{
    Size WinSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, WinSize.width/2);
    float y = MAX(position.y, WinSize.height/2);
    
    x = MIN(x, (_tileMap->getMapSize().width*_tileMap->getTileSize().width) - WinSize.width/2);
    y = MIN(y, (_tileMap->getMapSize().height*_tileMap->getTileSize().height) - WinSize.height/2);
    
    Vec2 actualPosition(x, y);
    Vec2 centerOfView(WinSize.width/2, WinSize.height/2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
    this->setPosition(viewPoint);
}

void TiledMapScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    const std::map<EventKeyboard::KeyCode, std::string>& keyTable = KeyTableScene::keyTable;
    if (keyTable.find(keyCode) == keyTable.end()) return;
    
    // PawnSprite movement
    // Plus delta position to move
    if (!keyTable.find(keyCode)->second.compare("Up"))
    {
        _player->addDeltaPosition(0.f, +_tileMap->getTileSize().height);
        _player->setCurrentDirection(PawnDirection::Vertical);
    }
    else if (!keyTable.find(keyCode)->second.compare("Down"))
    {
        _player->addDeltaPosition(0.f, -_tileMap->getTileSize().height);
        _player->setCurrentDirection(PawnDirection::Vertical);
    }
    else if (!keyTable.find(keyCode)->second.compare("Right"))
    {
        _player->addDeltaPosition(+_tileMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnDirection::Horizon);
    }
    else if (!keyTable.find(keyCode)->second.compare("Left"))
    {
        _player->addDeltaPosition(-_tileMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnDirection::Horizon);
    }
}

void TiledMapScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    const std::map<EventKeyboard::KeyCode, std::string>& keyTable = KeyTableScene::keyTable;
    
    // PawnSprite movement
    // Works as opposite onKeyPressed but no work for direction which is not concern on this function
    if (!keyTable.find(keyCode)->second.compare("Up"))
    {
        _player->addDeltaPosition(0.f, -_tileMap->getTileSize().height);
    }
    else if (!keyTable.find(keyCode)->second.compare("Down"))
    {
        _player->addDeltaPosition(0.f, +_tileMap->getTileSize().height);
    }
    else if (!keyTable.find(keyCode)->second.compare("Right"))
    {
        _player->addDeltaPosition(-_tileMap->getTileSize().width, 0.f);
    }
    else if (!keyTable.find(keyCode)->second.compare("Left"))
    {
        _player->addDeltaPosition(+_tileMap->getTileSize().width, 0.f);
    }
}

void TiledMapScene::update(float deltaTime)
{
    // Keep view-point center
    setViewPointCenter(_player->getPosition());
    
    // Player is valid && Player can do one action only && Player's delta position is not zero
    if (_player && _player->getNumberOfRunningActions() < 1 && !_player->getDeltaPositionOnDirection().equals(Vec2::ZERO))
    {
        const Vec2& newPosition = _player->getPosition() + _player->getDeltaPositionOnDirection();
        
        // Player can move only within world
        if (0.f <= newPosition.x && newPosition.x <= _tileMap->getTileSize().width*_tileMap->getMapSize().width &&
            0.f <= newPosition.y && newPosition.y <= _tileMap->getTileSize().height*_tileMap->getMapSize().height)
        {
            // player can't move to collidable tile
            if (!isCollidableTile(newPosition))
            {
                float duration = 0.125f;
                auto moveTo = MoveTo::create(duration, newPosition);
                _player->runAction(moveTo);
            }
        }
    }
}

Point TiledMapScene::getTileCoorForPosition(const cocos2d::Vec2& position)
{
    int x = position.x / _tileMap->getTileSize().width;
    // TileMap::'y' and cocos2d::'y' are opposite
    int y = ((_tileMap->getMapSize().height*_tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    
    return Point(x, y);
}

bool TiledMapScene::isCollidableTile(const cocos2d::Vec2& position)
{
    Point tileCoord = getTileCoorForPosition(position);
    int tileGid = _meta->getTileGIDAt(tileCoord);
    // Get the tile properties
    Value properties = _tileMap->getPropertiesForGID(tileGid);
    if (!properties.isNull())
    {
        ValueMap propsMap = properties.asValueMap();
        auto collidable = propsMap.find("Collidable");
        if (collidable != propsMap.end() && collidable->second.asBool())
        {
            return true;
        }
    }
    
    return false;
}
