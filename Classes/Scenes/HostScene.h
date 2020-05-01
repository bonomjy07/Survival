
#ifndef HOST_SCENE_H__
#define HOST_SCENE_H__

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class HostScene : public cocos2d::Layer
{
public:
    virtual ~HostScene();
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(HostScene);

private:
    Label *_roomTitleLabel;
    ui::TextField *_roomTitleTextField;
    Label *_roomPasswordLabel;
    ui::TextField *_roomPasswordText;
    Label *_roomCapacityLabel;
    ui::TextField *_roomCapacityText;
    Label *_roomPublicLabel;
    ui::CheckBox *_publicCheckBox;
    /**
     @brief Called if 'apply' button is pressed
     */
    void makeRoom();
    
    /**
     @brief Called if 'Back' button is pressed,
     or clicked 'ESC'
     */
    void goBack();
};

#endif // HOST_SCENE_H__