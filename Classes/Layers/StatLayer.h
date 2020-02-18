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

#endif /* StatLayer_h */

class StatLayer : public cocos2d::Layer
{
public:
    static StatLayer* create(const class Stat& stat);
    bool init();
    
public:
    StatLayer(const class Stat& stat);
    virtual ~StatLayer();
    
private:
    const class Stat& _stat;
};
