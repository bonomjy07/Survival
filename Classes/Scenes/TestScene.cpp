
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
#include "Tool.h"
#include "KeyBinder.h"

#include "InputController.h"

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
    
    //_spawnArea = objectGroup->getObject("SpawnArea");
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
        deerMeatSprite2->setName("Im_groot");
        this->addChild(deerMeatSprite2);
    }
    
    auto sword = ItemSprite::create();
    if ( sword ){
        auto item = Sword::create();
        sword->setItem(item);
        sword->setPosition(x - 48.f, y + 16.f);
        sword->initPhysicsBody();
        sword->setName("Im_sword");
        this->addChild(sword);
    }
    // Create player character
    /*
    auto p = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f);
    if (p)
    {
        p->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(p);
    }
     */
    
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
    
    
    //auto eventListner = EventListenerCustom::create("SpawnUnit", CC_CALLBACK_1(GameLayer::fun, this));
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(event)
    
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
        SurvivorSprite* player = getPlayerSprite(Multi::SOCKET_ID);
        InputController* controller = nullptr;
        if (player && (controller = player->getInputController()))
        {
            if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
            {
                controller->takeAction("Up", InputController::InputEvent::KeyPressed);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
            {
                controller->takeAction("Down", InputController::InputEvent::KeyPressed);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
            {
                controller->takeAction("Right", InputController::InputEvent::KeyPressed);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
            {
                controller->takeAction("Left", InputController::InputEvent::KeyPressed);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Collect") )
            {
                controller->takeAction("Collect", InputController::InputEvent::KeyPressed);
            }
        }
    }
    else if (Multi::ROLE_STATUS == Multi::Role::Client)
    {
        auto multi = getMulti();
        ValueMap data = ValueMap();
        data["ID"] = Multi::SOCKET_ID;
        data["type"] = "keyPressed";
        
        if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
        {
            data["action"] = "Up";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
        {
            data["action"] = "Down";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
        {
            data["action"] = "Right";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
        {
            data["action"] = "Left";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Collect") )
        {
            data["action"] = "Collect";
        }
        
        if (data.find("action") != data.end())
            multi->emit("action", data);
    }

    if ( gameKeyBinder->checkGameKeyAction(keyCode, "Use") )
    {
        getPlayerSprite()->useItemOnHand();
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
       toggleInventoryUI();
    }
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    if (Multi::ROLE_STATUS == Multi::Role::None || Multi::ROLE_STATUS == Multi::Role::Host)
    {
        SurvivorSprite* player = getPlayerSprite(Multi::SOCKET_ID);
        InputController* controller = nullptr;
        if (player && (controller = player->getInputController()))
        {
            if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
            {
                controller->takeAction("Up", InputController::InputEvent::KeyReleased);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
            {
                controller->takeAction("Down", InputController::InputEvent::KeyReleased);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
            {
                controller->takeAction("Right", InputController::InputEvent::KeyReleased);
            }
            else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
            {
                controller->takeAction("Left", InputController::InputEvent::KeyReleased);
            }
        }
    }
    else if (Multi::ROLE_STATUS == Multi::Role::Client)
    {
        auto multi = getMulti();
        ValueMap data = ValueMap();
        data["ID"] = Multi::SOCKET_ID;
        data["type"] = "keyReleased";

        //if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
        if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
        {
            data["action"] = "Up";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
        {
            data["action"] = "Down";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
        {
            data["action"] = "Right";
        }
        else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
        {
            data["action"] = "Left";
        }
        
        if (data.find("action") != data.end())
            multi->emit("action", data) ;
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

void TestScene::toggleInventoryUI(){
    auto parent = static_cast<Scene*>(getParent());
    if (auto inventoryLayer = parent->getChildByName("InventoryLayer"))
    {
        parent->removeChild(inventoryLayer);
    }
    // Create inventory layer if it doens't exist
    else if (auto inventoryLayer = InventoryLayer::create())
    {
        inventoryLayer->setInventory((getPlayerSprite()->getInventory()));
        parent->addChild(inventoryLayer);
    }
}
