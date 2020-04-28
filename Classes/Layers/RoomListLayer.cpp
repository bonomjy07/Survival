//
//  HostListLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#include "RoomListLayer.h"


Scene* RoomListLayer::createScene()
{
    auto scene = Scene::create();
    RoomListLayer* layer = RoomListLayer::create();
    scene->addChild(layer);
    return scene;
}

bool RoomListLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Vector<MenuItem*> labels;
    // Create Exit Label
    if (auto label = Label::createWithSystemFont("Back", "fonts/arial", 16))
    {
        if (auto exitLabel = MenuItemLabel::create(label, CC_CALLBACK_0(RoomListLayer::onBack, this)))
        {
            labels.pushBack(exitLabel);
        }
    }
    // Create Refresh Label
    if (auto label = Label::createWithSystemFont("Refresh", "fonts/arial", 16))
    {
        if (auto refreshLabel = MenuItemLabel::create(label, CC_CALLBACK_0(RoomListLayer::onRefresh, this)))
        {
            labels.pushBack(refreshLabel);
        }
    }
    // Create Menu
    if (auto menu = Menu::createWithArray(labels))
    {
        menu->alignItemsHorizontally();
        menu->setPosition(400.f, 200.f);
        this->addChild(menu);
    }

    return true;
}

void RoomListLayer::onBack()
{
    if (auto director = Director::getInstance())
    {
        director->popScene();
    }
}

#define HOST_LAYOUT 0x01
void RoomListLayer::onRefresh()
{
    ValueMap data;
    data["MyID"] = Multi::SOCKET_ID;
    _multi->emit("RefreshHosts", data); // multi'll call updateHosts
    
    // Remove previous host layout list;
    this->removeChildByTag(HOST_LAYOUT);
    // Add host layout on this layer
    for (Host host : _hostList)
    {
        // Create host
        RoomLayout* hostLayout = RoomLayout::create();
        hostLayout->updateHostLayout(host);
        addChild(hostLayout);
    }
}

void RoomListLayer::setMuilti(class Multi *multi)
{
    this->_multi = multi;
}

void RoomListLayer::updateHosts(std::vector<struct Host>& hostList)
{
    _hostList = hostList;
}
