#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "TestScene.h"
#include "SurvivorSprite.h"

#include "InputController.h"

#if BHY_DEBUG
#define SEVER_URI "localhost:8080"
#else 
#define SEVER_URI "192.168.219.102:8080"
#endif

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
    if ( !(_client = SocketIO::connect(SEVER_URI, *this)) ){
        return false;
    }

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("action", CC_CALLBACK_2(Multi::onAction, this));
    _client->on("doAction", CC_CALLBACK_2(Multi::doAction, this));

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
    log(args.c_str());

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
        }
        else
        {
            ROLE_STATUS = Role::Client;
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

    // Broadcasts player list to all clients
    auto dataVector = ValueVector();

    for (auto player : layer->getPlayersManager())
    {
        auto data = ValueMap();
        data["ID"] = player.first;
        data["x"] = player.second->getPositionX();
        data["y"] = player.second->getPositionY();

        dataVector.push_back(Value(data));
    }
    emit("playerList", dataVector);
}

void Multi::onPlayerList(cocos2d::network::SIOClient *client, const std::string &data)
{
    auto layer = getParentLayer();

    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        for (const auto& ele : document.GetArray())
        {
            std::string ID = ele["ID"].GetString();
            float x = ele["x"].GetFloat();
            float y = ele["y"].GetFloat();
            layer->addPlayerSpriteInWorld(ID, {x,y});
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

        /*
         if (type == 'input') {
            ctrl->takeAction(action, KeyPressed);
         }
         else if (type == 'prorperty') {
            sprite = getSprite(ID);
            sprite->updateNewProps(list);
         }
         else if (type == 'movement') {
            sprite = getSprite(ID);
            sprite->moveThePawn();
         }
         */
        
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
        
        /*
        // key pressed action
        if ( !type.compare("keyPressed") )
            layer->onMovePressed(id, action);
        // key released action
        else if ( !type.compare("keyReleased") )
            layer->onMoveReleased(id, action);
         */
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
            std::string type = document["type"].GetString();
            
            auto playerSprite = layer->getPlayerSprite(id);
            
            auto ctrl = playerSprite->getInputController();
            
             if ( !type.compare("keyPressed") ){
                ctrl->takeAction(action, InputController::InputEvent::KeyPressed);
            }
            // key released action
            else if ( !type.compare("keyReleased") ){
                ctrl->takeAction(action, InputController::InputEvent::KeyReleased);
            }
        }
    }
}

GameLayer* Multi::getParentLayer(){
    return static_cast<GameLayer*>(getParent());
}
