//
//  HostLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#include "HostLayout.h"
#include "HostListLayer.h"
#include "Multi.h"

USING_NS_CC;

bool HostLayout::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    // Name
    _ownerLabel = Label::createWithSystemFont("name label!", "arial", 16);
    if (_ownerLabel)
    {
        //TODO: position ...
        addChild(_ownerLabel);
    }

    // ID
    _titleLabel = Label::createWithSystemFont("ID label!", "arial", 16);
    if (_titleLabel)
    {
        addChild(_titleLabel);
    }
    
    // Button
    if (auto label = Label::createWithSystemFont("Connect", "arial", 16))
    {
        _connectBtn = MenuItemLabel::create(label, CC_CALLBACK_0(HostLayout::onClicked, this));
        if (_connectBtn)
        {
            addChild(_connectBtn);
        }
    }

    return true;
}

void HostLayout::onClicked()
{
    if (HostListLayer* parent = dynamic_cast<HostListLayer*>(_parent))
    {
        if (Multi* multi = parent->getMuilti())
        {
            ValueMap data;
            data["Title"] = _titleLabel->getString();
            data["Owner"] = _ownerLabel->getString();
            multi->emit("refresh-rooms", data);
        }
    }
}

void HostLayout::updateHostLayout(const Host& host)
{
    _titleLabel->setString(host.title);
    _ownerLabel->setString(host.owner);
}
