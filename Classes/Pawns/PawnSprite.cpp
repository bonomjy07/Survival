//
//  Pawn.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "PawnSprite.h"

#include <string>

USING_NS_CC;

PawnSprite* PawnSprite::create(const std::string& filename, const float initialHealth)
{
    PawnSprite* sprite = new (std::nothrow) PawnSprite(initialHealth);
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PawnSprite::PawnSprite(float initialHealth) : currentHealth(initialHealth), deltaPosition(0.f, 0.f)
{
    log("Pawn is created!");
}

void PawnSprite::setCurrentHealth(float newHealth)
{
    currentHealth = newHealth;
}

float PawnSprite::getCurrentHealth() const
{
    return currentHealth;
}

void PawnSprite::takeDamage(float damagedHealth)
{
    setCurrentHealth(currentHealth - damagedHealth);
}

void PawnSprite::addDeltaPosition(float x, float y)
{
    deltaPosition.x += x;
    deltaPosition.y += y;
}

void PawnSprite::setCurrentDirection(const PawnDirection& newDirection)
{
    currentDirection = newDirection;
}

const PawnDirection& PawnSprite::getCurrentDirection() const
{
    return currentDirection;
}

Vec2 PawnSprite::getDeltaPositionOnDirection() const
{
    // Diagnal movement is not allowed
    if (fabs(deltaPosition.x) == fabs(deltaPosition.y))
    {
        // Return delta position without x or y on direction
        if (currentDirection == PawnDirection::Horizon)
        {
            return Vec2(deltaPosition.x, 0);
        }
        else if (currentDirection == PawnDirection::Vertical)
        {
            return Vec2(0, deltaPosition.y);
        }
    }
    
    // If it's not diagnal, just return delta position
    return deltaPosition;
}

const Vec2& PawnSprite::getDeltaPosition() const
{
    return deltaPosition;
}

void PawnSprite::initPhysics()
{
    auto physcisBody = PhysicsBody::createBox(Size(32.f, 32.f),  PhysicsMaterial(0.1f, 1.0f, 0.0f));
    this->addComponent(physcisBody);
    this->getPhysicsBody()->setCategoryBitmask(0x01);
    this->getPhysicsBody()->setCollisionBitmask(0x02);
    //this->addComponent(physcisBody);
}

