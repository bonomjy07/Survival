
#include "GameLayer.h"
#include "SurvivorSprite.h"

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

void GameLayer::setClient(cocos2d::network::SIOClient* client)
{
    _client = client;
}

void GameLayer::addPlayerSpriteInWorld(const std::string &ID)
{
    if (auto player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f))
    {
        player->_ID = ID;
        // TODO: Make it simple...
        TMXObjectGroup* objectGroup = _tiledMap->getObjectGroup("Objects");
        if (objectGroup)
        {
            ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
            float x = spawnPoint["x"].asFloat();
            float y = spawnPoint["y"].asFloat();
            player->setPosition({x, y});
            addChild(player);
            _playersManager.insert({ID, player});
        }
    }
}

void GameLayer::addPlayerSpriteInWorld(const std::string &ID, const Vec2& position)
{
    if (auto player = SurvivorSprite::create("res/TestResource/TileImage/img_test_player.png", 100.f))
    {
        player->_ID = ID;
        player->setPosition(position);
        addChild(player);
        _playersManager.insert({ID, player});
    }
}

void GameLayer::onRequestPlayerID(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        _myID = document["MyID"].GetString();
        if (_myID.compare(document["HostID"].GetString()) == 0)
        {
            _role = Role::Host;
        }
        else
        {
            _role = Role::Client;
        }
        
        auto playerList = document["PlayerList"].GetArray();
        for (auto it = playerList.begin(); it != playerList.end(); ++it)
        {
            std::string ID = it->GetString();
            addPlayerSpriteInWorld(ID, {336, 368});
        }
    }
}

void GameLayer::onNewPlayer(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        std::string ID = document["ID"].GetString();
        addPlayerSpriteInWorld(ID, {336, 368});
    }
}

void GameLayer::onPawnMove(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        std::string ID = document["ID"].GetString();
        Vec2 newPosition(document["x"].GetFloat(), document["y"].GetFloat());
        auto playerSprite = getPlayerSprite(ID);
        playerSprite->moveThePawn(newPosition);
    }
}

void GameLayer::onMovePressed(cocos2d::network::SIOClient *client, const std::string &data)
{
    // get direction
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        std::string ID = document["ID"].GetString();
        std::string direction = document["direction"].GetString();
        if (auto playerSprite = getPlayerSprite(ID))
        {
            if (!direction.compare("up"))
            {
                playerSprite->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
                playerSprite->insertDirection(PawnSprite::Direction::Up);
            }
            else if (!direction.compare("down"))
            {
                playerSprite->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
                playerSprite->insertDirection(PawnSprite::Direction::Down);
            }
            else if (!direction.compare("right"))
            {
                playerSprite->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
                playerSprite->insertDirection(PawnSprite::Direction::Right);
            }
            else if (!direction.compare("left"))
            {
                playerSprite->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
                playerSprite->insertDirection(PawnSprite::Direction::Left);
            }
        }
    }
}

void GameLayer::onMoveReleased(cocos2d::network::SIOClient* client, const std::string& data)
{
    // get direction
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto ID = document["ID"].GetString();
        std::string direction = document["direction"].GetString();
        if (auto playerSprite = getPlayerSprite(ID))
        {
            if (!direction.compare("up"))
            {
                playerSprite->addDeltaPosition(0.f, -_tiledMap->getTileSize().height);
                playerSprite->eraseDirection(PawnSprite::Direction::Up);
            }
            else if (!direction.compare("down"))
            {
                playerSprite->addDeltaPosition(0.f, +_tiledMap->getTileSize().height);
                playerSprite->eraseDirection(PawnSprite::Direction::Down);
            }
            else if (!direction.compare("right"))
            {
                playerSprite->addDeltaPosition(-_tiledMap->getTileSize().width, 0.f);
                playerSprite->eraseDirection(PawnSprite::Direction::Right);
            }
            else if (!direction.compare("left"))
            {
                playerSprite->addDeltaPosition(+_tiledMap->getTileSize().width, 0.f);
                playerSprite->eraseDirection(PawnSprite::Direction::Left);
            }
        }
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
    if (_playersManager.find(_myID) == _playersManager.end())
        return nullptr;
    return _playersManager.at(_myID);
}

std::string& GameLayer::getMyID()
{
    return _myID;
}

void GameLayer::setMyID(const std::string& newID)
{
    _myID = newID;
}

cocos2d::network::SIOClient* GameLayer::getClient()
{
    return _client;
}

void GameLayer::setClient(cocos2d::network::SIOClient* client)
{
    _client = client;
}
