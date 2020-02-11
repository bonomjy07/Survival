//
//  MainScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//

#include "MainScene.h"
#include "TestScene.h"
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
    
    Vector<MenuItem*> menuItems;
    // Create menu start label
    if (auto startLabel = Label::createWithSystemFont("New Game Start", "arial", 16))
    {
        if (auto menuStartLabel = MenuItemLabel::create(startLabel, CC_CALLBACK_0(MainScene::startCallback, this)))
        {
            menuItems.pushBack(menuStartLabel);
        }
    }
    // Create menu key binding label
    if (auto keyBindinglabel = Label::createWithSystemFont("Key Setting", "arial", 16))
    {
        if (auto menuKeyBindingLael = MenuItemLabel::create(keyBindinglabel, CC_CALLBACK_0(MainScene::keyBindingCallback, this)))
        {
            menuItems.pushBack(menuKeyBindingLael);
        }
    }
    // Create menu exix label
    if (auto exitLabel = Label::createWithSystemFont("Exit", "arial", 16))
    {
        if (auto menuExitLabel = MenuItemLabel::create(exitLabel, CC_CALLBACK_0(MainScene::exitGameCallback, this)))
        {
            menuItems.pushBack(menuExitLabel);
        }
    }
    
    // Create menu with menu items
    if (auto menu = Menu::createWithArray(menuItems))
    {
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    return true;
}

void MainScene::startCallback()
{
    log("Gane start!");
    
    auto director = Director::getInstance();
    auto tiledMapScene = TestScene::createScene();
    director->pushScene(director->getRunningScene());
    director->replaceScene(tiledMapScene);
}

void MainScene::keyBindingCallback()
{
    auto director = Director::getInstance();
    auto keyTableScene = KeyTableScene::createScene();
    director->pushScene(director->getRunningScene());
    director->replaceScene(keyTableScene);
}

void MainScene::exitGameCallback()
{
    Director::getInstance()->end();
}

void MainScene::onEnterCallback()
{
    log("entered!");
}

