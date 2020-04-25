
#ifndef HOST_SCENE_H__
#define HOST_SCENE_H__

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HostScene : public cocos2d::Layer
{
public:
    virtual ~HostScene();
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(HostScene);

private:
    
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