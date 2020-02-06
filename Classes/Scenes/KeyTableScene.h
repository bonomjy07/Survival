//
//  KeyTableScene.h
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//
#ifndef KeyTableScene_h
#define KeyTableScene_h

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"

#endif /* KeyTableScene_h */

class keyTableScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(keyTableScene);
    
public:
    static std::map<cocos2d::EventKeyboard::KeyCode, std::string> keyTable;
    
private:
    cocos2d::Menu* buttonMenu;
    cocos2d::MenuItemLabel* applyMenuLabel;
    cocos2d::MenuItemLabel* backMenuLabel;

    cocos2d::Menu* keyTableMenu;
    cocos2d::Vector<cocos2d::MenuItem*> keyCodeTextField;
    cocos2d::Vector<cocos2d::MenuItem*> keyCommandLabels;
    
    /*
     @brief Called if 'apply' button is pressed
     */
    void applyNewKeyTable();
    
    /*
     @brief Called if 'Back' button is pressed,
     or clicked 'ESC'
     */
    void goBack();
    
};
