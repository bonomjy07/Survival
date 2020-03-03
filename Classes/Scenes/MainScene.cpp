//
//  MainScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//
#include <unistd.h>

#include "MainScene.h"
#include "TestScene.h"
#include "KeyTableScene.h"
#include "network/SocketIO.h"

#include "Multi.h"
#include "SpawnManager.h"

#include "Stat.h"

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
        startLabel->setTextColor(Color4B::YELLOW);
        if (auto menuStartLabel = MenuItemLabel::create(startLabel, CC_CALLBACK_0(MainScene::onStartGame, this)))
        {
            menuItems.pushBack(menuStartLabel);
        }
    }
    // Create Host label
    if (auto hostLabel = Label::createWithSystemFont("Host", "arial", 16))
    {
        if (auto menuHostLabel = MenuItemLabel::create(hostLabel, CC_CALLBACK_0(MainScene::onHostGame, this)))
        {
            menuItems.pushBack(menuHostLabel);
        }
    }
    // Create textField
    _uriTextField = cocos2d::ui::TextField::create("Enter URI", "arial", 16);
    if (_uriTextField)
    {
        _uriTextField->setMaxLength(20);
        _uriTextField->setPosition(Vec2(200, 100));
        addChild(_uriTextField);
    }
    // Create Enter label
    if (auto enterLabel = Label::createWithSystemFont("Enter", "arial", 16))
    {
        if (auto menuEnterLabel = MenuItemLabel::create(enterLabel, CC_CALLBACK_0(MainScene::onEnterGame, this)))
        {
            menuItems.pushBack(menuEnterLabel);
        }
    }
    // Create menu key binding label
    if (auto keyBindinglabel = Label::createWithSystemFont("Key Setting", "fonts/arial", 16))
    {
        if (auto menuKeyBindingLael = MenuItemLabel::create(keyBindinglabel, CC_CALLBACK_0(MainScene::onKeyBinding, this)))
        {
            menuItems.pushBack(menuKeyBindingLael);
        }
    }
    // Create menu exix label
    if (auto exitLabel = Label::createWithSystemFont("Exit", "fonts/arial", 16))
    {
        if (auto menuExitLabel = MenuItemLabel::create(exitLabel, CC_CALLBACK_0(MainScene::onExitGame, this)))
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

void MainScene::onStartGame() // Single....
{
    log("Game start!");
    
    if (auto tiledMapScene = TestScene::createScene())
    {
        //if (auto gameLayer = dynamic_cast<GameLayer*>(tiledMapScene->getChildByName("GameLayer")))
        auto gameLayer = dynamic_cast<GameLayer*>(tiledMapScene->getChildByName("GameLayer"));
        {
            gameLayer->setName("");
            Multi::ROLE_STATUS = Multi::Role::None;
            gameLayer->addPlayerSpriteInWorld("");
            // gameLayer->_treeManager->spawnTheSprite(10);
        }

        auto director = Director::getInstance();
        director->pushScene(director->getRunningScene());
        director->replaceScene(tiledMapScene);
    }
}

void MainScene::onKeyBinding()
{
    if (auto keyTableScene = KeyTableScene::createScene())
    {
        auto director = Director::getInstance();
        director->pushScene(director->getRunningScene());
        director->replaceScene(keyTableScene);
    }
}

void MainScene::onExitGame()
{
    Director::getInstance()->end();
}

void MainScene::onHostGame()
{
}

void MainScene::onEnterGame()
{
    // Reads URI
    std::string uri = _uriTextField->getString();
    //uri = "172.30.1.46:8080";
    uri = "192.168.219.101:8080";
    
    // Create gamelayer first, cuz gameLayer has delegate for client
    if (auto testScene = TestScene::createScene())
    {
        if (auto gameLayer = dynamic_cast<GameLayer*>(testScene->getChildByName("GameLayer")))
        {
            //auto multi = Multi::create(uri);
            auto multi = Multi::create();
            if ( multi ){
                multi->setName("MultiGame");
                gameLayer->addChild(multi);
                
                auto director = Director::getInstance();
                director->pushScene(director->getRunningScene());
                director->replaceScene(testScene);
            }
        }
    }
}
