//
//  MainScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//

#include "MainScene.h"
#include "TiledMapScene.h"
#include "KeyTableScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    
    MainScene* layer = MainScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // Create start label
    auto startLabel = Label::createWithSystemFont("Start!", "arial", 16);
    auto menuStartLabel = MenuItemLabel::create(startLabel, CC_CALLBACK_0(MainScene::startCallback, this));
    menuItems.pushBack(menuStartLabel);
    
    // Create key binding label
    auto label = Label::createWithSystemFont("Key setting", "arial", 16);
    auto menuLabel = MenuItemLabel::create(label, CC_CALLBACK_0(MainScene::keyBindingCallback, this));
    menuLabel->setPosition(100, 100);
    menuItems.pushBack(menuLabel);
    
    menu = Menu::createWithArray(menuItems);
    this->addChild(menu);
    
    return true;
}

void MainScene::startCallback()
{
    log("start!");
    
    auto director = Director::getInstance();
    auto tiledMapScene = TiledMapScene::createScene();
    
    auto currScene = director->getRunningScene();
    director->pushScene(currScene);
    director->replaceScene(tiledMapScene);
}

void MainScene::keyBindingCallback()
{
    auto director = Director::getInstance();
    auto keyTableScene = keyTableScene::createScene();
    
    director->pushScene(director->getRunningScene());
    director->replaceScene(keyTableScene);
}

