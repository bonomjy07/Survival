//
//  StatLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/02/17.
//

#pragma once

#ifndef StatLayer_h
#define StatLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class StatLayer : public cocos2d::Layer
{
public:
    static StatLayer* create(const class Stat& stat);
    bool init();
    void update(float dt) override;

public:
    StatLayer(const class Stat& stat);
    virtual ~StatLayer();
    
private:
    const class Stat& _stat;
    cocos2d::ui::Text* _texts[4];
    cocos2d::ui::LoadingBar* _bars[4];
};
#endif /* StatLayer_h */
