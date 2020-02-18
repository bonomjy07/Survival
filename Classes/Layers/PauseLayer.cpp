//
//  PauseLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/16.
//

#include "PauseLayer.h"

USING_NS_CC;

bool PauseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    if (auto label = Label::createWithSystemFont("- Pause -", "arial", 32))
    {
        label->setPosition(winSize.width/2, winSize.height - label->getContentSize().height);
        this->addChild(label);
    }
    
    Vector<MenuItem*> menuItems;
    if (auto label = Label::createWithSystemFont("Quick Save", "arial", 16))
    {
        if ((_qSaveLabel = MenuItemLabel::create(label, CC_CALLBACK_0(PauseLayer::onQuickSave, this))))
        {
            menuItems.pushBack(_qSaveLabel);
        }
    }
    if (auto label = Label::createWithSystemFont("Quick Load", "arial", 16))
    {
        if ((_qLoadLabel = MenuItemLabel::create(label, CC_CALLBACK_0(PauseLayer::onQuickLoad, this))))
        {
            menuItems.pushBack(_qLoadLabel);
        }
    }
    if (auto label = Label::createWithSystemFont("Back To Main", "arial", 16))
    {
        if ((_backToMainLabel = MenuItemLabel::create(label, CC_CALLBACK_0(PauseLayer::onBackToMain, this))))
        {
            menuItems.pushBack(_backToMainLabel);
        }
    }
    if (auto label = Label::createWithSystemFont("Resume", "arial", 16))
    {
        if ((_resumeLabel = MenuItemLabel::create(label, CC_CALLBACK_0(PauseLayer::onResume, this))))
        {
            menuItems.pushBack(_resumeLabel);
        }
    }
    
    if (auto menu = Menu::createWithArray(menuItems))
    {
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    
    this->setName("PauseLayer");
    
    return true;
}

void PauseLayer::onQuickSave() {}
void PauseLayer::onQuickLoad() {}
void PauseLayer::onBackToMain()
{
    if (auto director = Director::getInstance())
    {
        director->popToRootScene();
    }
}

void PauseLayer::onResume()
{
    if (auto currentScene = Director::getInstance()->getRunningScene())
    {
        currentScene->removeChild(this);
    }
}
