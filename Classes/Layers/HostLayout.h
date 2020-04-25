//
//  HostLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#pragma once

#ifndef HostLayer_h
#define HostLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

struct Host
{
    std::string ID;
    std::string roomID;
    std::string name;
};

class HostLayout : public cocos2d::ui::Layout
{
    bool init() override;

    cocos2d::Label* _IDLabel;
    
    cocos2d::Label* _nameLabel;
    
    cocos2d::MenuItemLabel* _connectBtn;
    
    void onClicked();
        
public:
    void updateHostLayout(const Host& host);
        
public:
    CREATE_FUNC(HostLayout);
};

#endif /* HostLayer_h */
