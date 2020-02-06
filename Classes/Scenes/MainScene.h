//
//  MainScene.h
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//
#pragma once

#ifndef MainScene_h
#define MainScene_h

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"


#endif /* MainScene_h */

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
    
    void startCallback();

    void keyBindingCallback();
    
private:
    cocos2d::Menu* menu;
    cocos2d::Vector<cocos2d::MenuItem*> menuItems;
};
