#ifndef GAME_LAYER_H__
#define GAME_LAYER_H__

#include "cocos2d.h"
#include "network/SocketIO.h"

class GameLayer : public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
public:
    virtual bool init() override;
    
public:
    /**
     @brief Returns true if tile at postion is collidable
     */
    bool isCollidableTileForPosition(const cocos2d::Vec2& position);
    
    /**
     @brief Returns true if the position is within the world
     */
    bool isPositionWithinWorld(const cocos2d::Vec2& position);
    
    /**
     @brief Check if there is a object which has physicsBody at position
     @details If frontVec is passed, frontVec*tileSize will be added to position
     @param frontVec Front vector to check a tile in front of position
     @return Node* address of object which has physicsBody
     */
    cocos2d::Node* checkNodeAtPosition(const cocos2d::Vec2& position);
    cocos2d::Node* checkNodeAtPosition(const cocos2d::Vec2& position, const cocos2d::Vec2& frontVec);
    
    /**
     @brief Check if there is objects which have physicsBody at position.
     @return Nodes will be retruned.
     */
    void checkNodesAtPosition(const cocos2d::Vec2& position, cocos2d::Vector<Node*>* nodes);
    
protected:
    cocos2d::TMXTiledMap *_tiledMap;
    cocos2d::TMXLayer *_block;
    class SurvivorSprite* _player;
    class KeyBinder *gameKeyBinder;
    
protected:
public:
    /**
     @brief This function is Called every frame and set view-point on center
     */
    void setViewPointCenter(const cocos2d::Vec2 position);

protected:
    /**
     @brief Return tile-location on position
     */
    cocos2d::Point getTileCoorForPosition(const cocos2d::Vec2& position);
    
    /**
     @brief Callback function for queryPoint()
     @param node Acutal type is Node**
     @return true As always
     */
    bool onQueryPointNode(cocos2d::PhysicsWorld& world, cocos2d::PhysicsShape& shape, void* node);
    
    /**
     @brief Callback function for queryPoint()
     @param node Actual type is Vector<Node*>*
     @return true As always
     */
    bool onQueryPointNodes(cocos2d::PhysicsWorld& world, cocos2d::PhysicsShape& shape, void* nodes);
    
    /* 테스트용 */
public:
    enum class Role {Host=0, Client};
    Role _role;
    
protected:
    std::map<std::string, class SurvivorSprite*> _playersManager;
    cocos2d::network::SIOClient* _client;
    std::set<cocos2d::Vec2> _occupied;
    
public:
    std::set<cocos2d::Vec2>& getOccupied();
    
    cocos2d::network::SIOClient* getClient();
    void setClient(cocos2d::network::SIOClient* client);
    
    void addPlayerSpriteInWorld(const std::string& ID);
    void addPlayerSpriteInWorld(const std::string& ID, const cocos2d::Vec2& position);
    
    class SurvivorSprite* getPlayerSprite(const std::string& ID) const;
    class SurvivorSprite* getPlayerSprite() const;

public:
    /* Gets ID and Checks what role is */
    void onRequestPlayerID(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Adds new player to player list and braodcasts player list */
    void onNewPlayer(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Receives player list and create pawn about player list */
    void onPlayerList(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Broadcasts pawn's movement */
    void onPawnMove(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Send movement to host */
    void onMovePressed(cocos2d::network::SIOClient* client, const std::string& data);
    
    /* Send movement to host */
    void onMoveReleased(cocos2d::network::SIOClient* client, const std::string& data);

    void onClose(cocos2d::network::SIOClient* client) override {cocos2d::log("onClose()");}
    void onError(cocos2d::network::SIOClient* client, const std::string& data) override { cocos2d::log("onError : %s", data.c_str());}
    
public:
    class SpawnManager* _treeManager;
    
    void addManager();
};
#endif // GAME_LAYER_H__
