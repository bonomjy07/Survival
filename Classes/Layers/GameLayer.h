#ifndef GAME_LAYER_H__
#define GAME_LAYER_H__

#include "cocos2d.h"
#include "network/SocketIO.h"
#include "Multi.h"

class GameLayer : public cocos2d::Layer
{
public:
    virtual bool init() override;
    
public:
    /* Returns true if tile at postion is collidable */
    bool isCollidableTileForPosition(const cocos2d::Vec2& position);
    
    /* Returns true if the position is within the world */
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
    
private:
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
    
public:
    /* Return player sprite corresponding ID */
    class SurvivorSprite* getPlayerSprite(const std::string& ID) const;
    
    /* Return player sprite corresponding MY-ID */
    class SurvivorSprite* getPlayerSprite() const;
    
    /* Accessor function for occupied positions by player sprite */
    std::set<cocos2d::Vec2>& getOccupied();
    
    /* Accessor function for Multi play delegate */
    Multi* getMulti(){ return dynamic_cast<Multi*>(getChildByName("MultiGame")); }
    
protected:
    /* This function is Called every frame and set view-point on center */
    void setViewPointCenter(const cocos2d::Vec2 position);
    
    /* Return tile-location on position */
    cocos2d::Point getTileCoorForPosition(const cocos2d::Vec2& position);
    
protected:
    /**
     Stores player ID to sprite address
     @warning player deletion is not implemented...
     */

    /* World map */
    cocos2d::TMXTiledMap *_tiledMap;
    
    /* Block tiles in world map */
    cocos2d::TMXLayer *_block;
    
    /* My play sprite */
    class SurvivorSprite* _player;

    /* Key binder */
    class KeyBinder *gameKeyBinder;
    
    /* Stores player's position to synchronize player's position */
    std::set<cocos2d::Vec2> _occupied;
    
protected:
    /* Return random ID for sprites */
    std::string getRandomID();
    
    /* Return random points in spawn area as num */
    void getRandomPointsInArea(const cocos2d::ValueMap& spawnArea, std::set<cocos2d::Vec2>& points, int num);
    
    /* Return random points in box as num */
    void getRandomPointsInArea(const cocos2d::Vec2& origin, const cocos2d::Vec2 boxExtend, std::set<cocos2d::Vec2>& points, int num);
    
    /* Create sprite with file name corresponding class name, then Return the sprite */
    class MySprite* createSpriteToSpawn(const std::string& classname, const std::string& filename);
    
public:
    /* Spwan player sprite in the world, default position is hard coded */
    void addPlayerSpriteInWorld(const std::string& ID);
    
    /* Spwan player sprite at position in the world */
    void addPlayerSpriteInWorld(const std::string& ID, const cocos2d::Vec2& position);
    
    /* Spwan unit sprite at position in the world */
    void addUnitSpriteInWorld(const std::string& ID, const std::string& fileaname, const cocos2d::Vec2& position, const float health);

    /* Spwan item sprite at position in the world */
    void addItemSpriteInWorld(const std::string& ID, const std::string& itemtype, const cocos2d::Vec2& position);
    
    /* Spwan multiple sprite in the box */
    void addSpritesInBox(const std::string& classname, const std::string& filename, const cocos2d::Vec2& origin, const cocos2d::Vec2& boxExtend, int numOfSpawn);
};
#endif // GAME_LAYER_H__
