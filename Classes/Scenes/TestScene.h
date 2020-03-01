#ifndef TEST_SCENE_H__
#define TEST_SCENE_H__

#include "cocos2d.h"

#include "GameLayer.h"

class TestScene : public GameLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(TestScene);
    
public:
    void update(float dt) override;

private:
    /**
     @brief Keyboard listeneres manage pawn sprite movement
     */
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    /**
     @brief Keyboard listeneres manage pawn sprite movement
     */
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    void togglePauseUI();
    
    void toggleStatUI();

    class Multi* multi;
    class SurvivorSprite* _player2;
};

#endif // TEST_SCENE_H__
