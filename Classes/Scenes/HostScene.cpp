#include <string>

#include "ui/CocosGUI.h"

#include "HostScene.h"
#include "Multi.h"

USING_NS_CC;

HostScene::~HostScene()
{
    log("HostScene deleted");
}


Scene* HostScene::createScene()
{
    auto scene = Scene::create();
    
    HostScene* layer = HostScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool HostScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    // Create title label
    if (auto titleLabel = Label::createWithSystemFont("Make Room", "arial", 16))
    {
        titleLabel->setPosition(visibleOrigin.x + visibleSize.width/2,
                                visibleOrigin.y + visibleSize.height - titleLabel->getContentSize().height);
        this->addChild(titleLabel);
    }
    
    Vector<MenuItem*> menuItems;
    // Create menu apply label
    if (auto applyLabel = Label::createWithSystemFont("Make", "arial", 16))
    {
        if (auto menuApplyLabel = MenuItemLabel::create(applyLabel, CC_CALLBACK_0(HostScene::makeRoom, this)))
        {
            menuItems.pushBack(menuApplyLabel);
        }
    }
    // Create menu back label
    if (auto backLabel = Label::createWithSystemFont("Back", "arial", 16))
    {
        if (auto menuBackLabel = MenuItemLabel::create(backLabel, CC_CALLBACK_0(HostScene::goBack, this)))
        {
            menuItems.pushBack(menuBackLabel);
        }
    }
    // Create menu for 'apply' and 'back'
    if (auto menu = Menu::createWithArray(menuItems))
    {
        menu->setPosition(visibleOrigin.x + visibleSize.width/2, visibleOrigin.y + 30);
        menu->alignItemsHorizontally();
        this->addChild(menu);
    }

   

    
    return true;
}

void HostScene::makeRoom()
{
    log("make room");
    if (auto multi = dynamic_cast<Multi*>(this->getParent()->getChildByName("MultiGame"))){
        log("multi");
        ValueMap data = ValueMap();
        data["title"] = "rooom1";
        data["password"] = "1234";
        multi->emit("make-room", data);
    }
}

void HostScene::goBack()
{
    log("Back");
    auto director = Director::getInstance();
    director->popScene();
}