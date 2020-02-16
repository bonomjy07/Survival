
#include "TestScene.h"
#include "PawnSprite.h"
#include "KeyTableScene.h"
#include "SpawnManager.h"
#include "PauseLayer.h"

#include "Item.h"
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
    if (!Layer::init())
    {
        return false;
    }

    // Create tile map and layer in tile map
    _tiledMap = TMXTiledMap::create("res/TestResource/TileMap/test_tilemap.tmx");
    _meta = _tiledMap->getLayer("BlockLayer");
    _meta->setVisible(false);
    this->addChild(_tiledMap);
    
    // Get SpawnPoint location
    TMXObjectGroup* objectGroup = _tiledMap->getObjectGroup("Objects");
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
    SpawnManager* pawnManager = SpawnManager::create(spawnArea, "PawnSprite", "res/TestResource/TileImage/img_test_player.png");
    if (pawnManager)
    {
        this->addChild(pawnManager);
        pawnManager->startSpawn();
    }
    
    // Create player character
    _player = PawnSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f);
    if (_player)
    {
        _player->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(_player);
    }
    
    auto _player2 = PawnSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f);
    if (_player2)
    {
        _player2->setPosition(x + 16.f + 64.f, y + 16.f);
        this->addChild(_player2);
    }

    DeerMeat* deerMeat = new DeerMeat();
    log(deerMeat->getDescription());

    auto deerMeatSprite = Sprite::create(deerMeat->getImageFileName());
    if (deerMeatSprite)
    {
        deerMeatSprite->setScale(0.25);
        deerMeatSprite->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(deerMeatSprite);
        this->setViewPointCenter(deerMeatSprite->getPosition());
    }

    auto deerMeatSprite2 = ItemSprite::create();
    DeerMeat* deerMeat2 = new DeerMeat();
    if (deerMeatSprite2)
    {
        deerMeatSprite2->setItem(deerMeat2);
        deerMeatSprite2->setScale(0.25);
        deerMeatSprite2->setPosition(x + 48.f, y + 16.f); // Locate it center of tile.
        this->addChild(deerMeatSprite2);
        this->setViewPointCenter(deerMeatSprite2->getPosition());
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
    gameKeyBinder = KeyBinder::create();

    // Register keyboard listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Allow update(float dt) to be called so that pawns move
    this->scheduleUpdate();
    
    return true;
}

void TestScene::setViewPointCenter(const cocos2d::Vec2 position)
{
    Size WinSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, WinSize.width/2);
    float y = MAX(position.y, WinSize.height/2);
    
    x = MIN(x, (_tiledMap->getMapSize().width*_tiledMap->getTileSize().width) - WinSize.width/2);
    y = MIN(y, (_tiledMap->getMapSize().height*_tiledMap->getTileSize().height) - WinSize.height/2);
    
    Vec2 actualPosition(x, y);
    Vec2 centerOfView(WinSize.width/2, WinSize.height/2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
    this->setPosition(viewPoint);
}

void TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // PawnSprite movement
    // Plus delta position to move
    if ( gameKeyBinder->checkGameKeyAction(keyCode, "Up") )
    {
        _player->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
        _player->setCurrentDirection(PawnDirection::Up);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Down") )
    {
        _player->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
        _player->setCurrentDirection(PawnDirection::Down);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Right") )
    {
        _player->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnDirection::Right);
    }
    else if ( gameKeyBinder->checkGameKeyAction(keyCode, "Left") )
    {
        _player->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
        _player->setCurrentDirection(PawnDirection::Left);
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        if (auto scene = getParent())
        {
            auto child = scene->getChildByName("PauseLayer");
            if (!child)
            {
                if (auto pauseLayer = PauseLayer::create())
                {
                    auto scene = static_cast<Scene*>(this->getParent());
                    scene->addChild(pauseLayer);
                }
            }
            else
            {
                scene->removeChild(child);
            }
        }
    }
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    
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

void TestScene::update(float deltaTime)
{
    // Keep view-point center
    setViewPointCenter(_player->getPosition());
}

Point TestScene::getTileCoorForPosition(const cocos2d::Vec2& position)
{
    int x = position.x / _tiledMap->getTileSize().width;
    // TileMap::'y' and cocos2d::'y' are opposite
    int y = ((_tiledMap->getMapSize().height*_tiledMap->getTileSize().height) - position.y) / _tiledMap->getTileSize().height;
    
    return Point(x, y);
}

bool TestScene::isCollidableTileForPosition(const cocos2d::Vec2& position)
{
    Point tileCoord = getTileCoorForPosition(position);
    int tileGid = _meta->getTileGIDAt(tileCoord);
    // Get the tile properties
    Value properties = _tiledMap->getPropertiesForGID(tileGid);
    if (!properties.isNull())
    {
        ValueMap propsMap = properties.asValueMap();
        auto collidable = propsMap.find("collision");
        if (collidable != propsMap.end() && collidable->second.asBool())
        {
            return true;
        }
    }
    return false;
}

Node* TestScene::checkNodeAtPosition(const Vec2& actualPosition)
{
    Node* node = nullptr;
    if (auto testScene = static_cast<Scene*>(getParent()))
    {
        if (auto pWorld = testScene->getPhysicsWorld())
        {
            pWorld->queryPoint(CC_CALLBACK_3(TestScene::onQueryPoint, this), actualPosition, (void*)&node);
        }
    }
    return node;
}

bool TestScene::onQueryPoint(PhysicsWorld &world, PhysicsShape &shape, void *node)
{
    PhysicsBody* pBody;
    if (node && (pBody = shape.getBody()))
    {
        *static_cast<Node**>(node) = pBody->getNode();
    }
    return true;
}

TMXTiledMap* TestScene::getTiledMap() const
{
    return _tiledMap;
}

TMXLayer* TestScene::getMetaLayer() const
    {
        return _meta;
    }
    
TestScene* TestScene::getGameLayer()
{
    if (auto currentScene = Director::getInstance()->getRunningScene())
    {
        if (auto gameLayer = static_cast<TestScene*>(currentScene->getChildByName("GameLayer")))
        {
            return gameLayer;
        }
    }
    return nullptr;
}
