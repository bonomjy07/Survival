#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "TestScene.h"
#include "SurvivorSprite.h"
#include "ItemSprite.h"

#include "InputController.h"

USING_NS_CC;

Multi::Role Multi::ROLE_STATUS = Multi::Role::None;
std::string Multi::SOCKET_ID = "";

Multi::Multi(){}
Multi::~Multi(){}

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
    if ( !(_client = SocketIO::connect("localhost:8080", *this)) ){
        return false;
    }

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("action", CC_CALLBACK_2(Multi::onAction, this));

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
    cocos2d::log("onClose()");
}
void Multi::onError(SIOClient* client, const std::string& data){
    // SocketIO::failed
    cocos2d::log("onError : %s", data.c_str());
}

void Multi::emit(const std::string &eventname, const Value &data){
    std::string args = parseData(data);
    //log(args.c_str());

    _client->emit(eventname, args);
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
            if ( iter != dataVector.begin() )
                ret += ",";
            ret += parseData(*iter);
        }
        ret += "]";
    }
    else if ( type == Value::Type::MAP ) {
        ret += "{";
        auto dataMap = data.asValueMap();
        for ( auto iter=dataMap.begin(); iter!=dataMap.end(); iter++ ) {
            auto pair = (*iter);
            std::string key = pair.first;
            
            if ( iter != dataMap.begin() )
                ret += ",";
            ret += "\"" + key + "\"";
            ret += ":";
            ret += parseData(pair.second);
        }
        ret += "}";
    }
    else if ( type == Value::Type::STRING )
        ret += "\"" + data.asString() + "\"";
    else if ( type == Value::Type::FLOAT )
        ret += std::to_string(data.asFloat());
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
    for (const auto node : layer->getChildren())
    {
        auto data = ValueMap();
        
        // Base properties
        if (dynamic_cast<MySprite*>(node))
        {
            data["ID"] = node->getName();
            data["x"] = node->getPositionX();
            data["y"] = node->getPositionY();
            // Extra properties
            if (SurvivorSprite* player = dynamic_cast<SurvivorSprite*>(node))
            {
                data["ClassName"] = "SurvivorSprite";
                data["FileName"] = player->getTexture()->getPath();
                data["Health"] = player->getCurrentHealth();
            }
            else if (UnitSprite* unit = dynamic_cast<UnitSprite*>(node))
            {
                data["ClassName"] = "UnitSprite";
                data["FileName"] = unit->getTexture()->getPath();
                data["Health"] = unit->getCurrentHealth();
            }
            else if (ItemSprite* item = dynamic_cast<ItemSprite*>(node))
            {
                // TODO: .....
                continue;
                //data["ClassName"] = "ItemSprite";
                //data["FileName"] = item->getTexture()->getPath();
            }
            dataVector.push_back(Value(data));
        }
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
            float x = ele["x"].GetFloat();
            float y = ele["y"].GetFloat();
            std::string classname = ele["ClassName"].GetString();
            std::string filename = ele["FileName"].GetString();
            float health = ele["Health"].GetFloat();
            
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
                // TODO: ....
                // layer->addUnitSprite(ID, filename, {x, y}, health);
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
            auto playerSprite = layer->getPlayerSprite(ID);
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

GameLayer* Multi::getParentLayer(){
    return static_cast<GameLayer*>(getParent());
}
