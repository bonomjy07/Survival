//
//  TiledMapScene.h
//  MyGame
//
//  Created by jy_maeng on 2020/01/31.
//
#ifndef TiledMapScene_h
#define TiledMapScene_h

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"

#endif /* TiledMapScene_h */

enum class Direction
{
    Vertical,
    Horizon
};

class TiledMapScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(TiledMapScene);
    
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
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::TMXLayer *_background;
    cocos2d::TMXLayer *_meta;
    
    cocos2d::Sprite* _player;
};
