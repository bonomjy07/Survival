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
    _nameLabel = Label::createWithSystemFont("name label!", "arial", 16);
    if (_nameLabel)
    {
        //TODO: position ...
        addChild(_nameLabel);
    }

    // ID
    _IDLabel = Label::createWithSystemFont("ID label!", "arial", 16);
    if (_IDLabel)
    {
        addChild(_IDLabel);
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
            // TODO: Room id ?
            ValueMap data;
            data["HostID"] = _IDLabel->getString();
            multi->emit("connect", data);
        }
    }
}

void HostLayout::updateHostLayout(const Host& host)
{
    _nameLabel->setString(host.name);
    _IDLabel->setString(host.ID);
}
