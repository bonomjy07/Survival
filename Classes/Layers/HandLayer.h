//
//  HandLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/03/21.
//

#pragma once

#ifndef HandLayer_h
#define HandLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HandLayer : public cocos2d::Layer
{
    bool init() override;
    /** Image view for item image on player's hand */
    cocos2d::ui::ImageView* _image;
    
public:
    CREATE_FUNC(HandLayer);
    
public:
    void setImageViewFile(const std::string& filename);
};
#endif /* HandLayer_h */
