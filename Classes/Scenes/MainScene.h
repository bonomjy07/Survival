//
//  MainScene.h
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//
#pragma once

#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
    
private:
    void onStartGame();
    void onKeyBinding();
    void onExitGame();
};
#endif /* MainScene_h */
