//
//  KeyTableScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//

// Below code is default code.
//#include "KeyTableScene.hpp"

#include "KeyTableScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* keyTableScene::createScene()
{
    auto scene = Scene::create();
    
    keyTableScene* layer = keyTableScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool keyTableScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // Create title label
    auto titleLabel = Label::createWithSystemFont("key binding setting", "arial", 16);
    if (titleLabel)
    {
        titleLabel->setPosition(200, 100);
        this->addChild(titleLabel);
    }
    // Create apply label
    auto applyLabel = Label::createWithSystemFont("Apply", "arial", 16);
    if (titleLabel)
    {
        //applyLabel->setPosition(150, 100);
    }
    // Create back label
    auto backLabel = Label::createWithSystemFont("Back", "arial", 16);
    if (titleLabel)
    {
        //backLabel->setPosition(200, 100);
    }
    
    Vector<MenuItem*> items;
    applyMenuLabel = MenuItemLabel::create(applyLabel, CC_CALLBACK_0(keyTableScene::applyNewKeyTable, this));
    items.pushBack(applyMenuLabel);
    
    backMenuLabel = MenuItemLabel::create(backLabel, CC_CALLBACK_0(keyTableScene::goBack, this));
    backMenuLabel->setPosition(50, 50);
    items.pushBack(backMenuLabel);
    
    buttonMenu = Menu::createWithArray(items);
    this->addChild(buttonMenu);
    
    return true;
}

void keyTableScene::applyNewKeyTable()
{
    log("apply");
    
}

void keyTableScene::goBack()
{
    log("dads");
    auto director = Director::getInstance();
    director->popScene();
}
