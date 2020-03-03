
#include "GameLayer.h"
#include "SurvivorSprite.h"
#include "SpawnManager.h"

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

void GameLayer::addPlayerSpriteInWorld(const std::string &ID)
{
    if (auto player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f))
    {
        player->setName(ID);
        // TODO: Make it simple...
        TMXObjectGroup* objectGroup = _tiledMap->getObjectGroup("Objects");
        if (objectGroup)
        {
            ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
            float x = spawnPoint["x"].asFloat();
            float y = spawnPoint["y"].asFloat();
            player->setPosition({x+_tiledMap->getTileSize().width/2, y+_tiledMap->getTileSize().height/2});
            _playersManager.insert({ID, player});
            addChild(player);
        }
    }
}

void GameLayer::addPlayerSpriteInWorld(const std::string &ID, const Vec2& position)
{
    if (auto player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f))
    {
        player->setName(ID);
        player->setPosition(position);
        _occupied.insert(position);
        _playersManager.insert({ID, player});
        addChild(player);

        if ( !ID.compare(Multi::SOCKET_ID) )
            _player = player;
    }
}

class SurvivorSprite* GameLayer::getPlayerSprite(const std::string &ID) const
{
    if (_playersManager.find(ID) == _playersManager.end())
        return nullptr;
    return _playersManager.at(ID);
}

class SurvivorSprite* GameLayer::getPlayerSprite() const
{
    return _player;
}

void GameLayer::onMovePressed(std::string ID, std::string direction)
{
    if (auto playerSprite = getPlayerSprite(ID))
    {
        if (!direction.compare("Up"))
        {
            playerSprite->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
            playerSprite->insertDirection(PawnSprite::Direction::Up);
        }
        else if (!direction.compare("Down"))
        {
            playerSprite->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
            playerSprite->insertDirection(PawnSprite::Direction::Down);
        }
        else if (!direction.compare("Right"))
        {
            playerSprite->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
            playerSprite->insertDirection(PawnSprite::Direction::Right);
        }
        else if (!direction.compare("Left"))
        {
            playerSprite->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
            playerSprite->insertDirection(PawnSprite::Direction::Left);
        }
    }
    
}

void GameLayer::onMoveReleased(std::string ID, std::string direction)
{
    if (auto playerSprite = getPlayerSprite(ID))
    {
        if (!direction.compare("Up"))
        {
            playerSprite->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
            playerSprite->eraseDirection(PawnSprite::Direction::Up);
        }
        else if (!direction.compare("Down"))
        {
            playerSprite->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
            playerSprite->eraseDirection(PawnSprite::Direction::Down);
        }
        else if (!direction.compare("Right"))
        {
            playerSprite->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
            playerSprite->eraseDirection(PawnSprite::Direction::Right);
        }
        else if (!direction.compare("Left"))
        {
            playerSprite->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
            playerSprite->eraseDirection(PawnSprite::Direction::Left);
        }
    }
    
}

std::set<Vec2>& GameLayer::getOccupied()
{
    return _occupied;
}
