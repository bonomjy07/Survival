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
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
    
private:
    void onStartGame();
    void onHostGame();
    void onEnterGame();
    void onKeyBinding();
    void onExitGame();
    
public:
    cocos2d::ui::TextField* _uriTextField;
};

#endif /* MainScene_h */
