#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "TestScene.h"
#include "SurvivorSprite.h"

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
    _client->on("hello", CC_CALLBACK_2(Multi::onReceiveEvent, this));
    _client->on("action", CC_CALLBACK_2(Multi::onReceiveEvent2, this));

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("movePressed", CC_CALLBACK_2(Multi::onMovePressed, this));
    _client->on("moveReleased", CC_CALLBACK_2(Multi::onMoveReleased, this));
    _player2 = nullptr;
    return true;
}

bool Multi::init(std::string uri){
    if ( !(_client = SocketIO::connect(uri, *this)) ){
        return false;
    }
    _client->on("hello", CC_CALLBACK_2(Multi::onReceiveEvent, this));
    _client->on("action", CC_CALLBACK_2(Multi::onReceiveEvent2, this));

    _client->on("requestPlayerID", CC_CALLBACK_2(Multi::onRequestPlayerID, this));
    _client->on("newPlayer", CC_CALLBACK_2(Multi::onNewPlayer, this));
    _client->on("playerList", CC_CALLBACK_2(Multi::onPlayerList, this));
    _client->on("pawnMove", CC_CALLBACK_2(Multi::onPawnMove, this));
    _client->on("movePressed", CC_CALLBACK_2(Multi::onMovePressed, this));
    _client->on("moveReleased", CC_CALLBACK_2(Multi::onMoveReleased, this));
    _player2 = nullptr;
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

void Multi::onReceiveEvent(SIOClient* client , const std::string& data){
    static bool isMine = true;

    rapidjson::Document doc;
    doc.Parse(data.c_str());
    if ( isMine ) {
        _player_number = doc["value"].GetInt64();
        log(_player_number);
        isMine = false;
    }
    else{
        auto director = Director::getInstance();
        TestScene* testscene = static_cast<TestScene*>(director->getRunningScene()->getChildByName("GameLayer"));
        _player2 = static_cast<SurvivorSprite*>(testscene->getChildByName("player2"));
    }
    log("onReceiveEvent");
}

void Multi::onReceiveEvent2(SIOClient* client , const std::string& data){

    rapidjson::Document doc;
    doc.Parse(data.c_str());
    if ( !std::string("2").compare(doc["player"].GetString()) ) {
        if ( _player2 )
            _player2->collect();
    }
    log("onReceiveEvent2");
}

void Multi::sendText(){
    _client->emit("action", "[{\"player\": \"" + std::to_string(_player_number) + "\",\"value\":\"hihihihih\"}]");
    log("sendText");
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
    std::string playerList = "[";
    for (auto player : layer->getPlayersManager())
    {
        std::string ID, x, y, data;
        ID = ID+"\"ID\":"+"\""+player.first+"\"";
        x = x+"\"x\":"+std::to_string(player.second->getPositionX());
        y = y+"\"y\":"+std::to_string(player.second->getPositionY());
        data = data+"{"+ID+","+x+","+y+"}";
        playerList += data + ",";
    }
    playerList.erase(playerList.length()-1);
    playerList += "]";
    _client->emit("playerList", playerList);
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

void Multi::onMovePressed(cocos2d::network::SIOClient *client, const std::string &data)
{
    auto layer = getParentLayer();

    // get direction
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto id = document["ID"].GetString();
        auto direction = document["direction"].GetString();
        layer->onMovePressed(id, direction);
    }
}

void Multi::onMoveReleased(cocos2d::network::SIOClient* client, const std::string& data)
{
    auto layer = getParentLayer();

    // get direction
    rapidjson::Document document;
    document.Parse(data.c_str());
    if (!document.GetParseError())
    {
        auto id = document["ID"].GetString();
        auto direction = document["direction"].GetString();
        layer->onMoveReleased(id, direction);
    }
}

GameLayer* Multi::getParentLayer(){
    return static_cast<GameLayer*>(getParent());
}