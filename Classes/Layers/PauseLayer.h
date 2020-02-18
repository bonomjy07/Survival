//
//  PauseLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/02/16.
//

#pragma once

#ifndef PauseLayer_h
#define PauseLayer_h

#include "cocos2d.h"

class PauseLayer : public cocos2d::Layer
{
public:
    bool init() override;
    CREATE_FUNC(PauseLayer);

public:
    // PauseLayer();
    // virtual ~PauseLayer();

private:
    cocos2d::MenuItemLabel* _qSaveLabel;
    cocos2d::MenuItemLabel* _qLoadLabel;
    cocos2d::MenuItemLabel* _backToMainLabel;
    cocos2d::MenuItemLabel* _resumeLabel;
    
private:
    void onQuickSave();
    void onQuickLoad();
    void onBackToMain();
    void onResume();
};

#endif /* PauseLayer_h */
