//
//  TiledMapScene.cpp
//  MyGame
//
//  Created by jy_maeng on 2020/01/31.
//

// Below code is default code.
//#include "TiledMapScene.hpp"

#include "TiledMapScene.h"
#include <string>

USING_NS_CC;

Scene* TiledMapScene::createScene()
{
    auto scene = Scene::create();
    
    TiledMapScene* layer = TiledMapScene::create();
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
    _background = _tileMap->getLayer("Background");
    _meta = _tileMap->getLayer("Meta");
    //_meta->setVisible(false);
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
    
    // Create player character
    _player = Sprite::create("res/TileGameResources/Player.png");
    if (_player)
    {
        _player->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(_player);
        this->setViewPointCenter(_player->getPosition());
    }
    deltaPosition.setZero();
    
    // Register keyboard listener for player
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TiledMapScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(TiledMapScene::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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

void TiledMapScene::setPlayerPosition(const cocos2d::Vec2 posotion)
{
    _player->setPosition(posotion);
}

void TiledMapScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // Add delta Position and set direction
    if (keyCode == EventKeyboard::KeyCode::KEY_W)
    {
        deltaPosition.y += _tileMap->getTileSize().height;
        direction = Direction::Vertical;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
        deltaPosition.y -= _tileMap->getTileSize().height;
        direction = Direction::Vertical;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        deltaPosition.x += _tileMap->getTileSize().width;
        direction = Direction::Horizon;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
    {
        deltaPosition.x -= _tileMap->getTileSize().width;
        direction = Direction::Horizon;
    }
}

void TiledMapScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // Sub delta position and no task for direction
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
        {
            deltaPosition.y -= _tileMap->getTileSize().height;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_S)
        {
            deltaPosition.y += _tileMap->getTileSize().height;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            deltaPosition.x -= _tileMap->getTileSize().width;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            deltaPosition.x += _tileMap->getTileSize().width;
       }
   }
}

void TiledMapScene::update(float deltaTime)
{
    setViewPointCenter(_player->getPosition());
    
    // Move player to direction and multiple action is not allowed
    if (!deltaPosition.equals(Vec2(0.f, 0.f)) && _player->getNumberOfRunningActions() < 1)
    {
        Vec2 newPosition = _player->getPosition();
        
        // Diagnol movement is not allowed
        if (abs(deltaPosition.x) == abs(deltaPosition.y))
        {
            // Add recent direction only
            if (direction == Direction::Vertical)
            {
                newPosition.y += deltaPosition.y;
            }
            else if (direction == Direction::Horizon)
            {
                newPosition.x += deltaPosition.x;
            }
        }
        // If it's not diagnol movement, just add delta position to new position
        else
        {
            newPosition += deltaPosition;
        }
        
        // Can't move to collidable postion
        if (!isCollidableTile(newPosition))
        {
            // New postion has to be within the world.
            float duration = 0.125f;
            float worldWidth  = _tileMap->getMapSize().width  * _tileMap->getTileSize().width;
            float worldHeight = _tileMap->getMapSize().height * _tileMap->getTileSize().height;
            if (0.f <= newPosition.x && newPosition.x <= worldWidth &&
                0.f <= newPosition.y && newPosition.y <= worldHeight)
            {
                auto moveTo = MoveTo::create(duration, newPosition);
                _player->runAction(moveTo);
            }
        }
    }
}

Point TiledMapScene::getTileCoorForPosition(const cocos2d::Vec2& position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height*_tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    
    return Point(x, y);
}

bool TiledMapScene::isCollidableTile(const cocos2d::Vec2& position)
{
    Point tileCoord = getTileCoorForPosition(position);
    int tileGid = _meta->getTileGIDAt(tileCoord);
    if (tileGid)
    {
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
    }
    
    return false;
}




