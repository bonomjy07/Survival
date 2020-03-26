#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "TestScene.h"
#include "SurvivorSprite.h"
#include "ItemSprite.h"

#include "InputController.h"
#include "ItemSprite.h"
#include "MSManager.h"

#if BHY_DEBUG
#define SEVER_URI "localhost:8080"
#else 
#define SEVER_URI "192.168.219.100:8080"
#endif

USING_NS_CC;

Multi::Role Multi::ROLE_STATUS = Multi::Role::None;
std::string Multi::SOCKET_ID = "None";

Multi::Multi(){}
Multi::~Multi()
{
    //_client = nullptr;
    //_client.reset();
}

static Multi* create(std::string uri) {
    Multi *pRet = new(std::nothrow) Multi();
    if (pRet && pRet->init(uri)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Multi::init(){
    if ( !(_client = SocketIO::connect(SEVER_URI, *this)) ){
        return false;
    }

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("action", CC_CALLBACK_2(Multi::onAction, this));
    _client->on("doAction", CC_CALLBACK_2(Multi::doAction, this));
    
    _client->on("NewUnitHealth", CC_CALLBACK_2(Multi::onUnitNewHealth, this));
    _client->on("SpriteDeletion", CC_CALLBACK_2(Multi::onSpriteDeletion, this));

    return true;
}

bool Multi::init(std::string uri){
    if ( !(_client = SocketIO::connect(uri, *this)) ){
        return false;
    }

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("action", CC_CALLBACK_2(Multi::onAction, this));
    _client->on("doAction", CC_CALLBACK_2(Multi::doAction, this));

    _client->on("NewUnitHealth", CC_CALLBACK_2(Multi::onUnitNewHealth, this));
    _client->on("SpriteDeletion", CC_CALLBACK_2(Multi::onSpriteDeletion, this));
    
    return true;
}

void Multi::onConnect(SIOClient* client){
     // SocketIO::connect success
}

void Multi::onMessage(SIOClient* client, const std::string& data){
     // SocketIO::send receive
}
void Multi::onClose(SIOClient* client){
    // SocketIO::disconnect success
    _client.reset();
    cocos2d::log("onClose()");
}
void Multi::onError(SIOClient* client, const std::string& data){
    // SocketIO::failed
    cocos2d::log("onError : %s", data.c_str());
}

void Multi::emit(const std::string &eventname, const Value &data){
    std::string args = parseData(data);
    log("emit : %s", args.c_str());

    if (_client)
    {
        _client->emit(eventname, args);
    }
}

void Multi::emit(const std::string &eventname, const ValueMap &data){
    emit(eventname, Value(data));
}

void Multi::emit(const std::string &eventname, const ValueVector &data){
    emit(eventname, Value(data));
}

std::string Multi::parseData(const Value &data){
    std::string ret = "";
    auto type = data.getType();
    if ( type == Value::Type::VECTOR ) {
        ret += "[";
        auto dataVector = data.asValueVector();
        for ( auto iter=dataVector.begin(); iter<dataVector.end(); iter++ ) {
            ret += parseData(*iter);
            if ( iter+1 != dataVector.end() )
                ret += ",";
        }
        ret += "]";
    }
    else if ( type == Value::Type::MAP ) {
        ret += "{";
        auto dataMap = data.asValueMap();
        bool isFirst = true;
        for ( auto iter=dataMap.begin(); iter!=dataMap.end(); iter++ ) {
            auto pair = (*iter);
            std::string key = pair.first;
            
            if (parseData(pair.second).compare("")){
                if (!isFirst){
                    if (iter!=dataMap.begin())
                        ret += ",";
                }
                ret += "\"" + key + "\"";
                ret += ":";
                ret += parseData(pair.second);
                isFirst = false;
            }
        }
        ret += "}";
    }
    else if ( type == Value::Type::STRING ){
        if (data.asString().compare(""))
            ret += "\"" + data.asString() + "\"";
    }
    else if ( type == Value::Type::FLOAT )
        ret += std::to_string(data.asFloat());
    else if (type == Value::Type::BOOLEAN)
        ret += data.asBool()?"true":"false";
    else if (type == Value::Type::INTEGER)
        ret += std::to_string(data.asInt());
    // TODO : if data is other type, write code like above (float type)
    return ret;
}

void Multi::onRequestPlayerID(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        // Get ID and check if i'm the host
        SOCKET_ID = document["MyID"].GetString();
        if (SOCKET_ID.compare(document["HostID"].GetString()) == 0)
        {
            ROLE_STATUS = Role::Host;
            
            // This event lets the world spawn units
            EventCustom event("SpawnUnit");
            const void* id = SOCKET_ID.c_str();
            event.setUserData(const_cast<void*>(id));
            _eventDispatcher->dispatchEvent(&event);
        }
        else
        {
            ROLE_STATUS = Role::Client;
            
            // Ask host that player list
            ValueMap data;
            data["ID"] = Multi::SOCKET_ID;
            emit("newPlayer", data); // host'll respond with player list
        }
    }
}

void Multi::onNewPlayer(cocos2d::network::SIOClient* client, const std::string& data)
{
    GameLayer* layer = getParentLayer();
    
    // Get my ID
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        std::string ID = document["ID"].GetString();
        layer->addPlayerSpriteInWorld(ID, {336, 368});
    }
    
    auto dataVector = ValueVector();
    for (const auto pair : MSManager::_manager)
    {
        const auto type = pair.second.first;
        const auto sprite = pair.second.second;
        auto data = ValueMap();
        
        data["ID"] = sprite->getName();
        data["x"] = sprite->getPositionX();
        data["y"] = sprite->getPositionY();
        // Extra properties
        if (SurvivorSprite* player = dynamic_cast<SurvivorSprite*>(sprite))
        {
            data["ClassName"] = "SurvivorSprite";
            std::string filename = player->getTexture()->getPath();
            std::string::size_type n = filename.find("/res/");
            data["FileName"] = filename.substr(n+1);
            data["Health"] = player->getCurrentHealth();
        }
        else if (UnitSprite* unit = dynamic_cast<UnitSprite*>(sprite))
        {
            data["ClassName"] = "UnitSprite";
            std::string filename = unit->getTexture()->getPath();
            std::string::size_type n = filename.find("/res/");
            data["FileName"] = filename.substr(n+1);
            data["Health"] = unit->getCurrentHealth();
        }
        else if (ItemSprite* item = dynamic_cast<ItemSprite*>(sprite))
        {
            data["ClassName"] = "ItemSprite";
            data["ItemType"] = item->getItem()->getName();
            if (item->getParent() == layer)
                data["onLayer"] = true;
            else
                data["onLayer"] = false;
        }
        dataVector.push_back(Value(data));
        
    }
    emit("playerList", dataVector); // TODO:
}

void Multi::onPlayerList(cocos2d::network::SIOClient *client, const std::string &data)
{
    auto layer = getParentLayer();
    
    log("data :%s", data.c_str());
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        for (const auto& ele : document.GetArray())
        {
            std::string ID = ele["ID"].GetString();
            if (MSManager::hasMySprite(ID))
                continue;
            float x = ele["x"].GetFloat();
            float y = ele["y"].GetFloat();
            std::string classname = ele["ClassName"].GetString();
            std::string filename = "";
            float health = 0.f;

            if (ele.HasMember("FileName"))
                filename = ele["FileName"].GetString();
            if (ele.HasMember("Health"))
                health = ele["Health"].GetFloat();
            
            if (!classname.compare("SurvivorSprite"))
            {
                layer->addPlayerSpriteInWorld(ID, {x,y});
            }
            else if (!classname.compare("UnitSprite"))
            {
                layer->addUnitSpriteInWorld(ID, filename, {x, y}, health);
            }
            else if (!classname.compare("ItemSprite"))
            {
                std::string itemtype = ele["ItemType"].GetString();
                if (ele["onLayer"].GetBool())
                    layer->addItemSpriteInWorld(ID, itemtype, {x, y});
                else
                    ItemSprite::create(itemtype);
            }
        }
    }
}

void Multi::onPawnMove(cocos2d::network::SIOClient* client, const std::string& data)
{
    auto layer = getParentLayer();
    
    if (Multi::Role::Client == ROLE_STATUS)
    {
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (!document.GetParseError())
        {
            std::string ID = document["ID"].GetString();
            Vec2 newPosition(document["x"].GetFloat(), document["y"].GetFloat());
            PawnSprite::Direction direction = static_cast<PawnSprite::Direction>(document["direction"].GetInt());
            auto playerSprite = layer->getPlayerSprite(ID);
            playerSprite->setCurrentDirection(direction);
            playerSprite->moveThePawn(newPosition);
        }
    }
}

void Multi::onAction(cocos2d::network::SIOClient* client, const std::string& data){
    auto layer = getParentLayer();
    
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto id = document["ID"].GetString();
        auto action = document["action"].GetString();
        std::string type = document["type"].GetString();
        
        // key pressed action
        if ( !type.compare("keyPressed") )
        {
            auto player = layer->getPlayerSprite(id);
            auto ctrl = player->getInputController();
            ctrl->takeAction(action, InputController::InputEvent::KeyPressed);
        }
        // key released action
        else if ( !type.compare("keyReleased") )
        {
            auto player = layer->getPlayerSprite(id);
            auto ctrl = player->getInputController();
            ctrl->takeAction(action, InputController::InputEvent::KeyReleased);
        }
    }
}

void Multi::doAction(cocos2d::network::SIOClient* client, const std::string& data){
    auto layer = getParentLayer();

    if (Multi::Role::Client == ROLE_STATUS)
    {
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (!document.GetParseError())
        {
            auto id = document["ID"].GetString();
            auto action = document["action"].GetString();
            auto itemID = document["itemID"].GetString();
            Node* toUnit = nullptr;
            if (document.HasMember("toUnitID")){
                auto toUnitID = document["toUnitID"].GetString();
                toUnit = MSManager::getAsUnit(toUnitID);
            }
            
            auto playerSprite = layer->getPlayerSprite(id);
            
            auto itemSprite = MSManager::getAsItem(itemID);

            auto ctrl = playerSprite->getInputController();
            ctrl->takeAction(action, itemSprite, toUnit);

        }
    }
}

void Multi::onUnitNewHealth(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto id = document["ID"].GetString();
        auto newHealth = document["newHealth"].GetFloat();
        if (auto unit = dynamic_cast<UnitSprite*>(getParentLayer()->getChildByName(id)))
        {
            unit->setCurrentHealth(newHealth);
        }
    }
}
   
void Multi::onSpriteDeletion(cocos2d::network::SIOClient* client, const std::string& data)
{
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto id = document["ID"].GetString();
        if (auto unit = dynamic_cast<UnitSprite*>(getParentLayer()->getChildByName(id)))
        {
            unit->onDeath();
        }
    }
}

GameLayer* Multi::getParentLayer(){
    return static_cast<GameLayer*>(getParent());
}
