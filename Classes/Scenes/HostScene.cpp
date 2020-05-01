#include <string>

#include "ui/CocosGUI.h"

#include "HostScene.h"
#include "Multi.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    
    auto stdX = visibleOrigin.x + visibleSize.width/2;
    auto stdY = visibleOrigin.y + visibleSize.height;
    // Create title label
    if (auto titleLabel = Label::createWithSystemFont("Make Room", "arial", 16))
    {
        titleLabel->setPosition(stdX, stdY - titleLabel->getContentSize().height);
        this->addChild(titleLabel);
    }
    int x = 100;
    int y = 50;
    stdY -= 100;

    // Create room title label
    if (_roomTitleLabel = Label::createWithSystemFont("제목", "arial", 16))
    {
        _roomTitleLabel->setPosition(stdX - x, stdY - y);
        this->addChild(_roomTitleLabel);
    }
    if (_roomTitleTextField = ui::TextField::create("제목", "arial", 32))
    {
        _roomTitleTextField->setPosition({stdX, stdY - y});
        this->addChild(_roomTitleTextField);
    }

    // Create room pw label
    if (_roomPasswordLabel = Label::createWithSystemFont("비밀번호", "arial", 16))
    {
        _roomPasswordLabel->setPosition(stdX - x, stdY - 2*y);
        this->addChild(_roomPasswordLabel);
    }
    if (_roomPasswordText = ui::TextField::create("비밀번호", "arial", 32))
    {
        _roomPasswordText->setPosition({stdX, stdY - 2*y});
        _roomPasswordText->setPasswordEnabled(true);
        _roomPasswordText->setMaxLength(10);
        this->addChild(_roomPasswordText);
    }

    // Create capacity label
    if (_roomCapacityLabel = Label::createWithSystemFont("인원수", "arial", 16))
    {
        _roomCapacityLabel->setPosition(stdX - x, stdY - 3*y);
        this->addChild(_roomCapacityLabel);
    }
    if (_roomCapacityText = ui::TextField::create("인원수", "arial", 32))
    {
        _roomCapacityText->setPosition({stdX, stdY - 3*y});
        _roomCapacityText->setMaxLength(2);
        this->addChild(_roomCapacityText);
    }

    // Create public label
    if (_roomPublicLabel = Label::createWithSystemFont("공개허용", "arial", 16))
    {
        _roomPublicLabel->setPosition(stdX - x, stdY - 4*y);
        this->addChild(_roomPublicLabel);
    }
    if (_publicCheckBox = ui::CheckBox::create())
    {
        _publicCheckBox->setPosition({stdX, stdY - 4*y});
        this->addChild(_publicCheckBox);
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
        data["title"] = _roomTitleTextField->getString();
        data["password"] = _roomPasswordText->getString();
        log("title : %s\npassword : %s\n", (data["title"]).asString().c_str(), data["password"].asString().c_str());
        multi->emit("make-room", data);
    }
}

void HostScene::goBack()
{
    log("Back");
    auto director = Director::getInstance();
    director->popScene();
}
