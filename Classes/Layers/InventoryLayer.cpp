//
//  InventoryLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/08.
//

// Below code is default code.
//#include "InventoryLayer.hpp"

#include "InventoryLayer.h"
#include "GridView.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

InventoryLayer::~InventoryLayer(){}

bool InventoryLayer::init()
{
    if ( !Layer::init() )
        return false;
    this->setName("InventoryLayer");

    auto visibleSize = Director::getInstance()->getVisibleSize();

    gridView = GridView::create();
    
    gridView->setGrid(4,8);
    // position the label on the center of the screen
    gridView->setBackGroundColor(Color3B(160, 160, 160));
    gridView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    for(int i=0; i<gridView->getMaxCount(); i++){
        auto layout = ui::Layout::create();
        layout->setLayoutType(ui::Layout::Type::HORIZONTAL);
        layout->setSize(Size(50, 50));
        layout->setBackGroundColor(Color3B(200, 200, 200));
        layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        gridView->addChild(layout);
    }
    gridView->setAnchorPoint(Point(0.5f,0.5f));
    gridView->setPosition(Point(visibleSize.width/2, visibleSize.height*2/3));
    gridView->arrange();
    this->addChild(gridView);
    // moveCenter();
    return true;
}

float InventoryLayer::getWeight() const { return weight; }
void InventoryLayer::setWeight(float _weight){ weight = _weight; }

void InventoryLayer::moveCenter()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(visibleSize);
    auto gridSize = gridView->getContentSize();
    // this->setPosition(visibleSize.width/2 - gridSize.width/2, visibleSize.height/2);
}
