//
//  MySprite.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/18.
//

#include "MySprite.h"

USING_NS_CC;

MySprite::~MySprite() {}

bool MySprite::initPhysicsBody()
{
    PhysicsBody* pBody;
    if (!(pBody = PhysicsBody::createBox(Size(28.f, 28.f), PhysicsMaterial(0.1f, 1.0f, 0.0f))))
    {
        log("Failed to create physcis body on MySprite");
        return false;
    }
    pBody->setDynamic(false);
    this->addComponent(pBody);
    return true;
}
