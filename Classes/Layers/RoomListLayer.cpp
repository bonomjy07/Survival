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
    if (_multi)
    {
        ValueMap data;
        data["MyID"] = Multi::SOCKET_ID;
        _multi->emit("RefreshHosts", data); // multi'll call updateHosts
    }
}

void RoomListLayer::setMuilti(class Multi *multi)
{
    this->_multi = multi;
}

void RoomListLayer::updateRooms(std::vector<struct Room>& roomList)
{
    _roomList = roomList;
    
    // Remove previous host layout list;
    removeChildByTag(HOST_LAYOUT);
    // Add room layout on this layer
    for (Room room : _roomList)
    {
        if (RoomLayout* roomLayout = RoomLayout::create())
        {
            roomLayout->updateLayout(room);
            addChild(roomLayout);
        }
    }
}
