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
    void setViewPointCenter(const cocos2d::Vec2 position);
    void setPlayerPosition(const cocos2d::Vec2 posoition);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    void update(float deltaTime) override;

protected:
    /* Direction used not to allow diagnol movement
     * It retains recent direction. */
    Direction direction;
    cocos2d::Vec2 deltaPosition;

    cocos2d::Point getTileCoorForPosition(const cocos2d::Vec2& position);
    bool isCollidableTile(const cocos2d::Vec2& position);
    
private:
    cocos2d::TMXTiledMap *tileMap;
    cocos2d::TMXLayer *backgroundLayer;
    cocos2d::TMXLayer *blockLayer;
    
    cocos2d::Sprite* player;
};

#endif // TEST_SCENE_H__
