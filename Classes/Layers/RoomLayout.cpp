//
//  HostLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/04/24.
//

#include "RoomLayout.h"
#include "RoomListLayer.h"
#include "Multi.h"

USING_NS_CC;

bool RoomLayout::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    // Name
    _ownerLabel = Label::createWithSystemFont("name label!", "arial", 16);
    if (_ownerLabel)
    {
        _ownerLabel->setPosition(0, 0);
        addChild(_ownerLabel);
    }

    // ID
    _titleLabel = Label::createWithSystemFont("ID label!", "arial", 16);
    if (_titleLabel)
    {
        _titleLabel->setPosition(30, 30);
        addChild(_titleLabel);
    }
    
    _connectionBtn = ui::Button::create();
    if (_connectionBtn)
    {
        _connectionBtn->setTitleText("Connect");
        _connectionBtn->setPosition({60.f, 60.f});
        _connectionBtn->addClickEventListener(CC_CALLBACK_1(RoomLayout::onClicked, this));
        addChild(_connectionBtn);
    }

    return true;
}

void RoomLayout::onClicked(Ref* ref)
{
    if (RoomListLayer* parent = dynamic_cast<RoomListLayer*>(_parent))
    {
        if (Multi* multi = parent->getMuilti())
        {
            ValueMap data;
            data["Title"] = _titleLabel->getString();
            data["Owner"] = _ownerLabel->getString();
            //TODO: connectio.d.adaskjhdkas
        }
    }
}

void RoomLayout::updateLayout(const Room& host)
{
    _titleLabel->setString(host.title);
    _ownerLabel->setString(host.owner);
}
