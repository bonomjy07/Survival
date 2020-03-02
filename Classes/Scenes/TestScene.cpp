
#include "TestScene.h"
#include "SurvivorSprite.h"
#include "KeyTableScene.h"
#include "MainScene.h"
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
    // TODO: if statement for nullptr
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
    
    ValueMap spawnArea = objectGroup->getObject("SpawnArea");
    if ((_treeManager = SpawnManager::create(spawnArea, "UnitSprite", "res/tileSet/qubodup-bush_berries_0.png")))
    {
        this->addChild(_treeManager);
        _treeManager->startSpawn();
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

    // Allow update(float dt) to be called so that pawns move
    this->scheduleUpdate();
    
    return true;
}

void TestScene::update(float deltaTime)
{
    if (auto player = getPlayerSprite(Multi::SOCKET_ID))
    {
        setViewPointCenter(player->getPosition());
    }
}

void TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // Pawn's movement
    if (Multi::ROLE_STATUS == Multi::Role::None || Multi::ROLE_STATUS == Multi::Role::Host)
    {
        if (auto player = getPlayerSprite(Multi::SOCKET_ID))
        {
            if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
            {
                player->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
                player->insertDirection(PawnSprite::Direction::Up);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
            {
                player->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
                player->insertDirection(PawnSprite::Direction::Down);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
            {
                player->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
                player->insertDirection(PawnSprite::Direction::Right);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
            {
                player->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
                player->insertDirection(PawnSprite::Direction::Left);
            }
        }
    }
    else if (Multi::ROLE_STATUS == Multi::Role::Client)
    {
        auto multi = getMulti();
        ValueMap data = ValueMap();
        data["ID"] = Multi::SOCKET_ID;
        data["type"] = "keyPressed";

        if (EventKeyboard::KeyCode::KEY_W == keyCode)
            data["action"] = "up";
        else if (EventKeyboard::KeyCode::KEY_S == keyCode)
            data["action"] = "down";
        else if (EventKeyboard::KeyCode::KEY_D == keyCode)
            data["action"] = "right";
        else if (EventKeyboard::KeyCode::KEY_A == keyCode)
            data["action"] = "left";
            
        multi->emit("action", data);
    }
    
    if ( gameKeyBinder->checkGameKeyAction(keyCode, "Collect") )
    {
        getPlayerSprite()->collect();
    }
    
    // ESC action
    if (EventKeyboard::KeyCode::KEY_ESCAPE == keyCode)
    {
        togglePauseUI();
    }
    
    // Stat layer
    if (EventKeyboard::KeyCode::KEY_TAB == keyCode)
    {
        toggleStatUI();
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
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    if (Multi::ROLE_STATUS == Multi::Role::None || Multi::ROLE_STATUS == Multi::Role::Host)
    {
        if (auto player = getPlayerSprite(Multi::SOCKET_ID))
        {
            if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
            {
                player->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
                player->eraseDirection(PawnSprite::Direction::Up);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
            {
                player->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
                player->eraseDirection(PawnSprite::Direction::Down);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
            {
                player->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
                player->eraseDirection(PawnSprite::Direction::Right);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
            {
                player->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
                player->eraseDirection(PawnSprite::Direction::Left);
            }
        }
    }
    else if (Multi::ROLE_STATUS == Multi::Role::Client)
    {
        auto multi = getMulti();
        ValueMap data = ValueMap();
        data["ID"] = Multi::SOCKET_ID;
        data["type"] = "keyReleased";

        if (EventKeyboard::KeyCode::KEY_W == keyCode)
            data["action"] = "up";
        else if (EventKeyboard::KeyCode::KEY_S == keyCode)
            data["action"] = "down";
        else if (EventKeyboard::KeyCode::KEY_D == keyCode)
            data["action"] = "right";
        else if (EventKeyboard::KeyCode::KEY_A == keyCode)
            data["action"] = "left";
            
        multi->emit("action", data);
    }
}

void TestScene::togglePauseUI()
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

void TestScene::toggleStatUI()
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
            if (auto statLayer = StatLayer::create(getPlayerSprite()->getStat()))
            {
                parent->addChild(statLayer);
                statLayer->scheduleUpdate();
            }
        }
    }
}
