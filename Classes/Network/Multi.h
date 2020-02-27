#ifndef MULTI_H__
#define MULTI_H__

#include "cocos2d.h"
#include "network/SocketIO.h"

USING_NS_CC;
using namespace cocos2d::network;

class Multi : public Ref, public SocketIO::SIODelegate
{
public:
    Multi();
    virtual ~Multi();
    virtual bool init();
    CREATE_FUNC(Multi);

    virtual void onConnect(SIOClient* client);
    virtual void onMessage(SIOClient* client, const std::string& data);
    virtual void onClose(SIOClient* client);
    virtual void onError(SIOClient* client, const std::string& data);

    void onReceiveEvent(SIOClient* client , const std::string& data);
    void sendText();

private:
    int player_number;
    SIOClient* _client;
};

#endif // MULTI_H__
