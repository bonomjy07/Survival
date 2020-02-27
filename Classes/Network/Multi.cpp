#include "Multi.h"
#include "json/rapidjson.h"
#include "json/document.h"

Multi::Multi(){
    init();
}
Multi::~Multi(){}

bool Multi::init(){
     _client = SocketIO::connect("localhost:3000", *this);
    _client->on("hello", CC_CALLBACK_2(Multi::onReceiveEvent, this));

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

    rapidjson::Document doc;
    doc.Parse(data.c_str());
    rapidjson::Value &val = doc["value"];
    //std::string value = val[rapidjson::SizeType(0)]["value"].GetString();
    log(doc["value"].GetString());
    log("onReceiveEvent");
};

void Multi::sendText(){
    _client->emit("hello", "[{\"value\":\"hihihihih\"}]");
    log("sendText");
}