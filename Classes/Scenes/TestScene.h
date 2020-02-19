#ifndef TEST_SCENE_H__
#define TEST_SCENE_H__

#include "cocos2d.h"

enum class Direction
{
    Vertical,
    Horizon
};

class TestScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(TestScene);
    
public:
    void update(float dt) override;
    
    /**
     @brief Called outside of class to get TestScene
     */
    static TestScene* getGameLayer();
    
    /**
     @brief Returns true if tile at postion is collidable
     */
    bool isCollidableTileForPosition(const cocos2d::Vec2& position);
    
    /**
     @brief Returns true if the position is within the world
     */
    bool isPositionWithinWorld(const cocos2d::Vec2& position);
    
    /**
     @brief Check if there is object which has physicsBody
     @details If frontVec is passed, frontVec*tileSize will be added to position
     @param frontVec Front vector to check a tile in front of position
     @return Node* address of object which has physicsBody
     */
    cocos2d::Node* checkNodeAtPosition(const cocos2d::Vec2& position);
    cocos2d::Node* checkNodeAtPosition(const cocos2d::Vec2& position, const cocos2d::Vec2& frontVec);
    void checkNodesAtPosition(const cocos2d::Vec2& position, cocos2d::Vector<Node*>* nodes);
    
private:
    cocos2d::TMXTiledMap *_tiledMap;
    cocos2d::TMXLayer *_meta;
    class SurvivorSprite* _player;
    class KeyBinder *gameKeyBinder;
    
private:
    /**
     @brief This function is Called every frame and set view-point on center
     */
    void setViewPointCenter(const cocos2d::Vec2 position);
    
    /**
     @brief Keyboard listeneres manage pawn sprite movement
     */
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    /**
     @brief Keyboard listeneres manage pawn sprite movement
     */
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
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
    bool onQueryPointNodes(cocos2d::PhysicsWorld& world, cocos2d::PhysicsShape& shape, void* nodes);
};

#endif // TEST_SCENE_H__
