#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "TestScene.h"
#include "SurvivorSprite.h"

USING_NS_CC;

Multi::Multi(){
    init();
}
Multi::~Multi(){}

bool Multi::init(){
    _client = SocketIO::connect("localhost:3000", *this);
    _client->on("hello", CC_CALLBACK_2(Multi::onReceiveEvent, this));
    _client->on("action", CC_CALLBACK_2(Multi::onReceiveEvent2, this));
    player2 = nullptr;
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
}
void Multi::onError(SIOClient* client, const std::string& data){
    // SocketIO::failed
}

void Multi::onReceiveEvent(SIOClient* client , const std::string& data){
    static bool isMine = true;

    rapidjson::Document doc;
    doc.Parse(data.c_str());
    if ( isMine ) {
        player_number = doc["value"].GetInt64();
        log(player_number);
        isMine = false;
    }
    else{
        auto director = Director::getInstance();
        TestScene* testscene = static_cast<TestScene*>(director->getRunningScene()->getChildByName("GameLayer"));
        player2 = static_cast<SurvivorSprite*>(testscene->getChildByName("player2"));
    }
    log("onReceiveEvent");
}

void Multi::onReceiveEvent2(SIOClient* client , const std::string& data){

    rapidjson::Document doc;
    doc.Parse(data.c_str());
    if ( !std::string("2").compare(doc["player"].GetString()) ) {
        if ( player2 )
            player2->collect();
    }
    log("onReceiveEvent2");
}

void Multi::sendText(){
    _client->emit("action", "[{\"player\": \"" + std::to_string(player_number) + "\",\"value\":\"hihihihih\"}]");
    log("sendText");
}