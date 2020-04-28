//
//  HostListLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#pragma once

#ifndef HostListLayer_h
#define HostListLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Multi.h"
#include "RoomLayout.h"

class RoomListLayer : public cocos2d::Layer
{
    bool init() override;
    
    /** Referece for multi manager */
    cocos2d::RefPtr<class Multi> _multi;
    
    /** Container for host  */
    std::vector<struct Room> _roomList;

    /** Refresh button listener */
    void onRefresh();
    
    /** Back button listener */
    void onBack();
    
public:
    inline Multi* getMuilti() const { return _multi;};
    
    /** Setter function for multi */
    void setMuilti(class Multi* multi);
    
    /** Updates host list based on hosts parameter */
    void updateRooms(std::vector<struct Room>& hostList);

public:
    CREATE_FUNC(RoomListLayer);
    static cocos2d::Scene* createScene();
};

#endif /* HostListLayer_h */
