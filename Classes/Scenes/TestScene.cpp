
#include "TestScene.h"
#include "SurvivorSprite.h"
#include "KeyTableScene.h"
#include "SpawnManager.h"
#include "PauseLayer.h"
#include "StatLayer.h"
#include "InventoryLayer.h"

#include "Multi.h"
#include "ItemSprite.h"
#include "Food.h"
#include "KeyBinder.h"

#include <string>

USING_NS_CC;

Scene* TestScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    if (auto pWorld = scene->getPhysicsWorld())
    {
        pWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
        pWorld->setGravity(Vec2(0.f, 0.f)); // No gravity
    }
    
    auto layer = TestScene::create();
    layer->setName("GameLayer");

    scene->addChild(layer);
    return scene;
}

bool TestScene::init()
{
    if (!GameLayer::init())
    {
        return false;
    }
    
    // Create tile map and layer in tile map
    _tiledMap = TMXTiledMap::create("res/TestResource/TileMap/test_tilemap.tmx");
    _block = _tiledMap->getLayer("BlockLayer");
    _block->setVisible(false);
    this->addChild(_tiledMap);
    
    // Get SpawnPoint location
    TMXObjectGroup* objectGroup = _tiledMap->getObjectGroup("Objects");
    if (objectGroup == NULL)
    {
        log("No object group naemd : Objects");
        return false;
    }

    // Create spawn manager for tree
    ValueMap spawnArea = objectGroup->getObject("SpawnArea");
    SpawnManager* pawnManager = SpawnManager::create(spawnArea, "TreeSprite", "res/tileset/qubodup-bush_berries_0.png");
    if (pawnManager)
    {
        this->addChild(pawnManager);
        pawnManager->startSpawn();
    }


    ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();


     // Create item sprite
    auto deerMeatSprite2 = ItemSprite::create();
    if (deerMeatSprite2)
    {
        auto item = DeerMeat::create();
        deerMeatSprite2->setItem(item);
        deerMeatSprite2->setPosition(x + 48.f, y + 16.f); // Locate it center of tile.
        deerMeatSprite2->initPhysicsBody();
        this->addChild(deerMeatSprite2);
    }

    // Create player character
    _player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f);
    if (_player)
    {
        _player->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(_player);
    }

    _player2 = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f);
    if (_player2)
    {
        _player2->setPosition(x + 48.f, y + 16.f); // Locate it center of tile.
        _player2->setName("player2");
        this->addChild(_player2);
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto label = Label::createWithTTF("Test World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // create key binder
    //KeyBinder::loadGameKeyActions();
    gameKeyBinder = new KeyBinder();

    // Register keyboard listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    multi = new Multi();


    // Allow update(float dt) to be called so that pawns move
    this->scheduleUpdate();
    
    return true;
}

void TestScene::update(float deltaTime)
{
    // Keep the player on center of view
    if (_player)
    {
        setViewPointCenter(_player->getPosition());
    }
}

void TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

    multi->sendText();
    if (!_player) return;
    
    // PawnSprite movement
    // Plus delta position to move
    if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
    {
        _player->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
        _player->setCurrentDirection(PawnSprite::Direction::Up);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
    {
        _player->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
        _player->setCurrentDirection(PawnSprite::Direction::Down);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
    {
        _player->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnSprite::Direction::Right);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
    {
        _player->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnSprite::Direction::Left);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Collect") )
    {
        _player->collect();
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Inventory") )
    {
        auto parent = static_cast<Scene*>(getParent());
        if (auto inventoryLayer = parent->getChildByName("InventoryLayer"))
        {
            parent->removeChild(inventoryLayer);
        }
        // Create inventory layer if it doens't exist
        else if (auto inventoryLayer = InventoryLayer::create())
        {
            inventoryLayer->setInventory((_player->getInventory()));
            parent->addChild(inventoryLayer);
        }
    }

    // ESC action
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        if (Scene* parent = static_cast<Scene*>(getParent()))
        {
            // Remove pause scene if it exists already
            if (auto pauseLayer = parent->getChildByName("PauseLayer"))
            {
                parent->removeChild(pauseLayer);
            }
            // Create pause scene if it doens't exist
            else
            {
                if (auto pauseLayer = PauseLayer::create())
                {
                    parent->addChild(pauseLayer);
                }
            }
        }
    }
    
    if (EventKeyboard::KeyCode::KEY_TAB == keyCode)
    {
        if (Scene* parent = static_cast<Scene*>(getParent()))
        {
            // Remove pause scene if it exists already
            if (auto statLayer = parent->getChildByName("StatLayer"))
            {
                parent->removeChild(statLayer);
            }
            // Create pause scene if it doens't exist
            else
            {
                if (auto statLayer = StatLayer::create(_player->getStat()))
                {
                    parent->addChild(statLayer);
                }
            }
        }
    }
    
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    
    if (!_player) return;
    // PawnSprite movement
    // Works as opposite onKeyPressed but no work for direction which is not concern on this function
    if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
    {
        _player->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
    {
        _player->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
    {
        _player->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
    {
        _player->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
    }
}
