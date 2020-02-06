

#include "TestScene.h"
#include <string>

USING_NS_CC;

Scene* TestScene::createScene()
{
    auto scene = Scene::create();
    
    TestScene* layer = TestScene::create();
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
    tileMap = TMXTiledMap::create("res/TestResource/TileMap/test_tilemap.tmx");
    backgroundLayer = tileMap->getLayer("BackgroundLayer");
    auto backgroundLayer2 = tileMap->getLayer("BackgroundLayer2");
    blockLayer = tileMap->getLayer("BlockLayer");
    blockLayer->setVisible(false);
    backgroundLayer->setVisible(true);
    backgroundLayer2->setVisible(true);
    this->addChild(tileMap);
    
    // Get SpawnPoint location
    TMXObjectGroup* objectGroup = tileMap->getObjectGroup("Objects");
    if (objectGroup == NULL)
    {
        log("No object group naemd : Objects");
        return false;
    }
    ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();
    
    // Create player character
    player = Sprite::create("res/TestResource/TileImage/img_test_player.png");
    if (player)
    {
        player->setPosition(x + 16.f, y + 16.f); // Locate it center of tile.
        this->addChild(player);
        this->setViewPointCenter(player->getPosition());
    }
    deltaPosition.setZero();
    
    // Register keyboard listener for player
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
    return true;
}

void TestScene::setViewPointCenter(const cocos2d::Vec2 position)
{
    Size WinSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, WinSize.width/2);
    float y = MAX(position.y, WinSize.height/2);
    
    x = MIN(x, (tileMap->getMapSize().width*tileMap->getTileSize().width) - WinSize.width/2);
    y = MIN(y, (tileMap->getMapSize().height*tileMap->getTileSize().height) - WinSize.height/2);
    
    Vec2 actualPosition(x, y);
    Vec2 centerOfView(WinSize.width/2, WinSize.height/2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
    this->setPosition(viewPoint);
}

void TestScene::setPlayerPosition(const cocos2d::Vec2 posotion)
{
    player->setPosition(posotion);
}

void TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // Add delta Position and set direction
    if (keyCode == EventKeyboard::KeyCode::KEY_W)
    {
        deltaPosition.y += tileMap->getTileSize().height;
        direction = Direction::Vertical;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
        deltaPosition.y -= tileMap->getTileSize().height;
        direction = Direction::Vertical;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        deltaPosition.x += tileMap->getTileSize().width;
        direction = Direction::Horizon;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
    {
        deltaPosition.x -= tileMap->getTileSize().width;
        direction = Direction::Horizon;
    }
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    // Sub delta position and no task for direction
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
        {
            deltaPosition.y -= tileMap->getTileSize().height;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_S)
        {
            deltaPosition.y += tileMap->getTileSize().height;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            deltaPosition.x -= tileMap->getTileSize().width;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            deltaPosition.x += tileMap->getTileSize().width;
       }
   }
}

void TestScene::update(float deltaTime)
{
    setViewPointCenter(player->getPosition());
    
    // Move player to direction and multiple action is not allowed
    if (!deltaPosition.equals(Vec2(0.f, 0.f)) && player->getNumberOfRunningActions() < 1)
    {
        Vec2 newPosition = player->getPosition();
        
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

        float worldWidth  = tileMap->getMapSize().width  * tileMap->getTileSize().width;
        float worldHeight = tileMap->getMapSize().height * tileMap->getTileSize().height;
        
        if (0.f <= newPosition.x && newPosition.x < worldWidth &&
                0.f <= newPosition.y && newPosition.y < worldHeight)
        {
            // Can't move to collidable postion
            if (!isCollidableTile(newPosition))
            {
                // New postion has to be within the world.
                float duration = 0.125f;
            
                auto moveTo = MoveTo::create(duration, newPosition);
                player->runAction(moveTo);
            }
        }
    }
}

Point TestScene::getTileCoorForPosition(const cocos2d::Vec2& position)
{
    int x = position.x / tileMap->getTileSize().width;
    int y = ((tileMap->getMapSize().height*tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height;
    
    return Point(x, y);
}

bool TestScene::isCollidableTile(const cocos2d::Vec2& position)
{
    Point tileCoord = getTileCoorForPosition(position);
    int tileGid = blockLayer->getTileGIDAt(tileCoord);
    if (tileGid)
    {
        Value properties = tileMap->getPropertiesForGID(tileGid);
        if (!properties.isNull())
        {
            ValueMap propsMap = properties.asValueMap();
            auto collidable = propsMap.find("collision");
            if (collidable != propsMap.end() && collidable->second.asBool())
            {
                return true;
            }
        }
    }
    
    return false;
}




