//
//  KeyTableScene.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/06.
//
#include "KeyBinder.h"
#include <string>


#include "KeyTableScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

std::map<cocos2d::EventKeyboard::KeyCode, std::string> KeyTableScene::keyTable;

void KeyTableScene::initKeyTable()
{
    KeyBinder::loadGameKeyActions();
    // keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_W, "Up"});
    // keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_S, "Down"});
    // keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_D, "Right"});
    // keyTable.insert({cocos2d::EventKeyboard::KeyCode::KEY_A, "Left"});
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

    canSetKey = false;
    curButton = nullptr;

    listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyTableScene::onKeyPressed, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Vector<ui::TextField*> textFields;
    Vector<Label*> labels;
    // Key binding list
    float y = 100.f;
    for (auto keyPair : KeyBinder::gameKeyTable)
    {
        std::string sKeyCode = KeyBinder::getKeyName(keyPair.first);
        auto btn = ui::Button::create();
        btn->setTitleText(sKeyCode);
        btn->setActionTag(static_cast<int>(keyPair.first));
        btn->addTouchEventListener(CC_CALLBACK_2(KeyTableScene::onButtonPressed, this));
        auto label2 = Label::createWithSystemFont(keyPair.second, "arial", 16);
        label2->setPosition(visibleSize.width/3, y);
        btn->Node::setPosition(visibleSize.width/3 + 100.f, y);
        y += label2->getContentSize().height;
        this->addChild(label2);
        this->addChild(btn);
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

void KeyTableScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    log("onKeyPressed");
    log(canSetKey?"true":"false");
    if( canSetKey && curButton != NULL ){
        EventKeyboard::KeyCode key = static_cast<EventKeyboard::KeyCode>(curButton->getActionTag());
        std::string action = gameKeyBinder->findGameKeyAction(key);
        log(action.c_str());
        if(!action.compare("None")){
            canSetKey = false;
            curButton = nullptr;
            return;
        }
        gameKeyBinder->setGameKeyAction(keyCode, action);
        curButton->setTitleText(KeyBinder::getKeyName(keyCode));
        curButton->setActionTag(static_cast<int>(keyCode));
    }
    canSetKey = false;
    curButton = nullptr;
}

void KeyTableScene::onButtonPressed(Ref* button, ui::Widget::TouchEventType eventType)
{
    log("onButtonPressed");
    switch(eventType){
        case ui::Widget::TouchEventType::BEGAN:
            curButton = (ui::Button*)button;
            canSetKey = true;
            break;
    }
}