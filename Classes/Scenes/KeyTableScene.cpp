//
//  KeyTableScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//

#include "KeyTableScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

std::map<cocos2d::EventKeyboard::KeyCode, std::string> KeyTableScene::keyTable;

void KeyTableScene::initKeyTable()
{
    keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_W, "Up"});
    keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_S, "Down"});
    keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_D, "Right"});
    keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_A, "Left"});
}

Scene* KeyTableScene::createScene()
{
    auto scene = Scene::create();
    
    KeyTableScene* layer = KeyTableScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool KeyTableScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    // Create title label
    if (auto titleLabel = Label::createWithSystemFont("Key Binding Setting", "arial", 16))
    {
        titleLabel->setPosition(visibleOrigin.x + visibleSize.width/2,
                                visibleOrigin.y + visibleSize.height - titleLabel->getContentSize().height);
        this->addChild(titleLabel);
    }
    
    Vector<MenuItem*> menuItems;
    // Create menu apply label
    if (auto applyLabel = Label::createWithSystemFont("Apply", "arial", 16))
    {
        if (auto menuApplyLabel = MenuItemLabel::create(applyLabel, CC_CALLBACK_0(KeyTableScene::applyNewKeyTable, this)))
        {
            menuItems.pushBack(menuApplyLabel);
        }
    }
    // Create menu back label
    if (auto backLabel = Label::createWithSystemFont("Back", "arial", 16))
    {
        if (auto menuBackLabel = MenuItemLabel::create(backLabel, CC_CALLBACK_0(KeyTableScene::goBack, this)))
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
    
    Vector<ui::TextField*> textFields;
    Vector<Label*> labels;
    // Key binding list
    float y = 100.f;
    for (auto keyPair : keyTable)
    {
        std::string sKeyCode = std::to_string(static_cast<int>(keyPair.first));
        auto textField = ui::TextField::create(sKeyCode, "arial", 16);
        auto label2 = Label::createWithSystemFont(keyPair.second, "arial", 16);
        label2->setPosition(visibleSize.width/2, y);
        textField->Node::setPosition(visibleSize.width/2 + 50.f, y);
        y += label2->getContentSize().height;
        this->addChild(label2);
        this->addChild(textField);
    }
    
    return true;
}

void KeyTableScene::applyNewKeyTable()
{
    log("apply new key binding");
}

void KeyTableScene::goBack()
{
    log("Back");
    auto director = Director::getInstance();
    director->popScene();
}
