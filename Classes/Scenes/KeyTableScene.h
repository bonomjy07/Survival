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
#include "ui/CocosGUI.h"

#endif /* KeyTableScene_h */

class KeyTableScene : public cocos2d::Layer
{
public:
    virtual ~KeyTableScene();
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(KeyTableScene);
    
protected:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onButtonPressed(cocos2d::Ref* button, cocos2d::ui::Widget::TouchEventType eventType);

private:
    cocos2d::Vector<cocos2d::Label*> keyCommandLabels;
    cocos2d::Vector<cocos2d::TextFieldTTF*> keyCodeTextFields;
    cocos2d::EventListenerKeyboard *listener;
    bool canSetKey;
    cocos2d::ui::Button *curButton;
    class KeyBinder *gameKeyBinder;
    /**
     @brief Called if 'apply' button is pressed
     */
    void applyNewKeyTable();
    
    /**
     @brief Called if 'Back' button is pressed,
     or clicked 'ESC'
     */
    void goBack();

    void showGameKeys();
};
