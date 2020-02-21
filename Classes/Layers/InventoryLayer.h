//
//  InventoryLayer.h
//  Survival
//
//  Created by jy_maeng on 2020/02/08.
//
#pragma once

#ifndef InventoryLayer_h
#define InventoryLayer_h

#include <stdio.h>
#include <iostream>

#include "cocos2d.h"

#include "GridView.h"

#endif /* InventoryLayer_h */

class InventoryLayer : public cocos2d::Layer
{
public:
    virtual ~InventoryLayer();
    virtual bool init() override;
    CREATE_FUNC(InventoryLayer);
    
public:
    //void addNewItem(class Item);
    
    float getWeight() const;
    void setWeight(float deltaWeight);

private:
    void moveCenter();
    
private:
    //cocos2d::Vector<class Item> inventoryItems;
    float weight;
    GridView *gridView;
};
