//
//  KeyTableScene.h
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//

#pragma once

#ifndef KeyTableScene_h
#define KeyTableScene_h

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"

#endif /* KeyTableScene_h */

class KeyTableScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(KeyTableScene);
    
public:
    static std::map<cocos2d::EventKeyboard::KeyCode, std::string> keyTable;
    static void initKeyTable();

private:
    cocos2d::Vector<cocos2d::Label*> keyCommandLabels;
    cocos2d::Vector<cocos2d::TextFieldTTF*> keyCodeTextFields;
    
    /**
     @brief Called if 'apply' button is pressed
     */
    void applyNewKeyTable();
    
    /**
     @brief Called if 'Back' button is pressed,
     or clicked 'ESC'
     */
    void goBack();
    
};
