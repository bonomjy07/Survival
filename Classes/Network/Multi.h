#ifndef MULTI_H__
#define MULTI_H__

#include "cocos2d.h"
#include "network/SocketIO.h"

USING_NS_CC;
using namespace cocos2d::network;

class Multi : public Node, public SocketIO::SIODelegate
{
public:
    enum class Role {None=0, Host, Client};
    static Role ROLE_STATUS;
    static std::string SOCKET_ID;

public:
    Multi();
    virtual ~Multi();
    virtual bool init();
    virtual bool init(std::string uri);
    CREATE_FUNC(Multi);
    static Multi* create(std::string uri);

    void sendText();

public:
    /* Gets ID and Checks what role is */
    void onRequestPlayerID(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Adds new player to player list and braodcasts player list */
    void onNewPlayer(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Receives player list and create pawn about player list */
    void onPlayerList(cocos2d::network::SIOClient* client, const std::string& data);

    /* Receive action from guest */
    void onAction(cocos2d::network::SIOClient* client, const std::string& data);

    /* Broadcasts pawn's movement */
    void onPawnMove(cocos2d::network::SIOClient* client, const std::string& data);
    
    virtual void onConnect(SIOClient* client);
    virtual void onMessage(SIOClient* client, const std::string& data);
    void onClose(cocos2d::network::SIOClient* client) override;
    void onError(cocos2d::network::SIOClient* client, const std::string& data) override;

    cocos2d::network::SIOClient* getClient(){ return _client; }
    void setClient(cocos2d::network::SIOClient* client){ _client = client; }

private:
    class GameLayer* getParentLayer();

private:
    SIOClient* _client;
};

#endif // MULTI_H__
