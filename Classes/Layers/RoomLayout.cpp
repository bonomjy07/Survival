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
    _ownerLabel = Label::createWithSystemFont("owner:", "arial", 16);
    if (_ownerLabel)
    {
        _ownerLabel->setPosition(0, 60.f);
        addChild(_ownerLabel);
    }

    // ID
    _titleLabel = Label::createWithSystemFont("title:", "arial", 16);
    if (_titleLabel)
    {
        _titleLabel->setPosition(0, 30.f);
        addChild(_titleLabel);
    }
    
    _connectionBtn = ui::Button::create();
    if (_connectionBtn)
    {
        _connectionBtn->setTitleText("Connection Button");
        _connectionBtn->setPosition({0.f, 0.f});
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
    _titleLabel->setString("title : " + host.title);
    _ownerLabel->setString("owner : " + host.owner);
}
