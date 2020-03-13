
#include "GameLayer.h"
#include "SurvivorSprite.h"
#include "SpawnManager.h"
#include "ItemSprite.h"

#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

bool GameLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

bool GameLayer::isCollidableTileForPosition(const cocos2d::Vec2& position)
{
    Point tileCoord = getTileCoorForPosition(position);
    int tileGid = _block->getTileGIDAt(tileCoord);
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

bool GameLayer::isPositionWithinWorld(const Vec2 &position)
{
    if ((0.f <= position.x && position.x <= _tiledMap->getTileSize().width*_tiledMap->getMapSize().width) &&
        (0.f <= position.y && position.y <= _tiledMap->getTileSize().height*_tiledMap->getMapSize().height))
    {
       return true;
   }
    return false;
}

Node* GameLayer::checkNodeAtPosition(const Vec2& position)
{
    Node* node = nullptr;
    if (auto testScene = dynamic_cast<Scene*>(getParent()))
    {
        if (auto pWorld = testScene->getPhysicsWorld())
        {
            Vec2 actualPosition = position + this->getPosition();
            pWorld->queryPoint(CC_CALLBACK_3(GameLayer::onQueryPointNode, this), actualPosition, (void*)&node);
        }
    }
    return node;
}

Node* GameLayer::checkNodeAtPosition(const Vec2& position, const Vec2& frontVec)
{
    Node* node = nullptr;
    if (auto testScene = dynamic_cast<Scene*>(getParent()))
    {
        if (auto pWorld = testScene->getPhysicsWorld())
        {
            Vec2 actualPosition = position + this->getPosition() + _tiledMap->getTileSize().width*frontVec;
            pWorld->queryPoint(CC_CALLBACK_3(GameLayer::onQueryPointNode, this), actualPosition, (void*)&node);
        }
    }
    return node;
}

void GameLayer::checkNodesAtPosition(const Vec2& position, Vector<Node*>* nodes)
{
    if (auto testScene = dynamic_cast<Scene*>(getParent()))
    {
        if (auto pWorld = testScene->getPhysicsWorld())
        {
            Vec2 actualPosition = position + this->getPosition();
            pWorld->queryPoint(CC_CALLBACK_3(GameLayer::onQueryPointNodes, this), actualPosition, (void*)nodes);
        }
    }
}


bool GameLayer::onQueryPointNode(PhysicsWorld &world, PhysicsShape &shape, void *node)
{
    PhysicsBody* pBody;
    if (node && (pBody = shape.getBody()))
    {
        *static_cast<Node**>(node) = pBody->getNode();
    }
    return true;
}

bool GameLayer::onQueryPointNodes(PhysicsWorld& world, PhysicsShape& shape, void* nodes)
{
    PhysicsBody* pBody;
    if (nodes && (pBody = shape.getBody()))
    {
        Vector<Node*>* tmpNodes = static_cast<Vector<Node*>*>(nodes);
        tmpNodes->pushBack(pBody->getNode());
    }
    return true;
}


class SurvivorSprite* GameLayer::getPlayerSprite(const std::string &ID) const
{
    return dynamic_cast<SurvivorSprite*>(getChildByName(ID));
}

class SurvivorSprite* GameLayer::getPlayerSprite() const
{
    return dynamic_cast<SurvivorSprite*>(getChildByName(Multi::SOCKET_ID));
}

std::set<Vec2>& GameLayer::getOccupied()
{
    return _occupied;
}

void GameLayer::setViewPointCenter(const cocos2d::Vec2 position)
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

Point GameLayer::getTileCoorForPosition(const cocos2d::Vec2& position)
{
    int x = position.x / _tiledMap->getTileSize().width;
    // TileMap::'y' and cocos2d::'y' are opposite
    int y = ((_tiledMap->getMapSize().height*_tiledMap->getTileSize().height) - position.y) / _tiledMap->getTileSize().height;
    
    return Point(x, y);
}

std::string GameLayer::getRandomID()
{
    static std::set<std::string> IDManager;
    static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    std::string newID;
    
    do {
        newID.clear();
        int len = RandomHelper::random_int(1, 30);
        for (int i = 0; i < len; ++i)
        {
            newID += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
    } while (IDManager.find(newID) != IDManager.end());
    IDManager.insert(newID);
    
    return newID;
}

void GameLayer::getRandomPointsInArea(const cocos2d::ValueMap& spawnArea, std::set<cocos2d::Vec2>& points, int num)
{
    while (points.size() < num)
    {
        float x = spawnArea.at("x").asFloat();
        float y = spawnArea.at("y").asFloat();
        float width = spawnArea.at("width").asFloat();
        float height = spawnArea.at("height").asFloat();
        
        float randX = floor(RandomHelper::random_real(x, x+width));
        float randY = floor(RandomHelper::random_real(y, y+height));
        // Trim random position as tile position
        randX = randX - ((int)randX % 32) + 32/2;
        randY = randY - ((int)randY % 32) + 32/2;
        
        points.insert({randX, randY});
    }
}

void GameLayer::getRandomPointsInArea(const cocos2d::Vec2& origin, const cocos2d::Vec2 boxExtend, std::set<cocos2d::Vec2>& points, int num)
{
    while (points.size() < num)
    {
        float randX = floor(RandomHelper::random_real(origin.x, boxExtend.x));
        float randY = floor(RandomHelper::random_real(origin.y, boxExtend.y));
        // Trim random position as tile position
        randX = randX - ((int)randX % 32) + 32/2;
        randY = randY - ((int)randY % 32) + 32/2;
        
        points.insert({randX, randY});
    }
}

MySprite* GameLayer::createSpriteToSpawn(const std::string& classname, const std::string& filename) 
{
    MySprite* sprite = nullptr;
    if (!classname.compare(""))
    {
        return sprite;
    }
    else if (!classname.compare("PawnSprite"))
    {
        sprite = PawnSprite::create(filename, 100.f);
    }
    else if (!classname.compare("UnitSprite"))
    {
        sprite = UnitSprite::create(filename);
    }
    return sprite;
}


void GameLayer::addPlayerSpriteInWorld(const std::string &ID)
{
    TMXObjectGroup* objectGroup = _tiledMap->getObjectGroup("Objects");
    Vec2 position = Vec2({0,0});

    if (objectGroup)
    {
        ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        position = Vec2({x+_tiledMap->getTileSize().width/2, y+_tiledMap->getTileSize().height/2});
    }

    addPlayerSpriteInWorld(ID, position);
}

void GameLayer::addPlayerSpriteInWorld(const std::string &ID, const Vec2& position)
{
    if (auto player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f))
    {
        player->setName(ID);
        player->setPosition(position);
        _occupied.insert(position);
        addChild(player);

        if ( !ID.compare(Multi::SOCKET_ID) )
        {
            _player = player;
        }
    }
}

void GameLayer::addUnitSpriteInWorld(const std::string& ID, const std::string& filename, const cocos2d::Vec2& position, const float health)
{
    if (auto unit = UnitSprite::create(filename, health))
    {
        unit->setName(ID);
        unit->setPosition(position);
        unit->setCurrentHealth(health);
        addChild(unit);
    }
}

void GameLayer::addItemSpriteInWorld(const std::string& ID, const std::string& itemtype, const cocos2d::Vec2& position)
{
    if (auto unit = ItemSprite::create(itemtype))
    {
        unit->setName(ID);
        unit->setPosition(position);
        addChild(unit);
    }
}

void GameLayer::addSpritesInBox(const std::string& classname, const std::string& filename, const cocos2d::Vec2& origin, const cocos2d::Vec2& boxExtend, int numOfSpawn)
{
    // Get random points to spawn
    std::set<Vec2> points;
    getRandomPointsInArea(origin, boxExtend, points, numOfSpawn);
    
    // Create sprite along with random point
    for (const auto& point : points)
    {
        if (auto sprite = createSpriteToSpawn(classname, filename)) // TODO: what about copy-ctor, not calling func
        {
            sprite->setName(getRandomID());
            sprite->setPosition(point);
            addChild(sprite);
        }
    }
}
