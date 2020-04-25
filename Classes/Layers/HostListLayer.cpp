//
//  HostListLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#include "HostListLayer.h"

bool HostListLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Vector<MenuItem*> labels;
    // Create Exit Label
    if (auto label = Label::createWithSystemFont("Exit", "fonts/arial", 16))
    {
        if (auto exitLabel = MenuItemLabel::create(label, CC_CALLBACK_0(HostListLayer::onBack, this)))
        {
            labels.pushBack(exitLabel);
        }
    }
    // Create Refresh Label
    if (auto label = Label::createWithSystemFont("Refresh", "fonts/arial", 16))
    {
        if (auto refreshLabel = MenuItemLabel::create(label, CC_CALLBACK_0(HostListLayer::onRefresh, this)))
        {
            labels.pushBack(refreshLabel);
        }
    }
    // Create Menu
    if (auto menu = Menu::createWithArray(labels))
    {
        menu->alignItemsHorizontally();
        this->addChild(menu);
    }

    return true;
}

void HostListLayer::onBack()
{
    if (auto director = Director::getInstance())
    {
        director->popScene();
    }
}

#define HOST_LAYOUT 0x01
void HostListLayer::onRefresh()
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
        HostLayout* hostLayout = HostLayout::create();
        hostLayout->updateHostLayout(host);
        this->addChild(hostLayout);
    }
}

void HostListLayer::setMuilti(class Multi *multi)
{
    this->_multi = multi;
}

void HostListLayer::updateHosts(std::vector<struct Host>& hostList)
{
    _hostList = hostList;
}
