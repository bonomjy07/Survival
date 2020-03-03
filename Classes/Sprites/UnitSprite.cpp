//
//  UnitSprite.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/18.
//

#include "UnitSprite.h"

USING_NS_CC;

UnitSprite* UnitSprite::create(const std::string& filename, float maxHealth)
{
    UnitSprite* sprite = new(std::nothrow) UnitSprite(maxHealth);
    if (sprite && sprite->initWithFile(filename) && sprite->initPhysicsBody())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

UnitSprite::UnitSprite(float maxHealth) : _maxHealth(maxHealth)
{
    _currentHealth = _maxHealth;
}

UnitSprite::~UnitSprite() {}

float UnitSprite::getCurrentHealth() const
{
    return _currentHealth;
}

void UnitSprite::setCurrentHealth(float newCurrentHealth)
{
    _currentHealth = newCurrentHealth;
}

void UnitSprite::takeDamage(float deltaDamage)
{
    setCurrentHealth(_currentHealth - deltaDamage);
    
    if (_currentHealth <= 0.f)
    {
        onDeath();
    }
}

void UnitSprite::onDeath()
{
    if (auto gameLayer = dynamic_cast<GameLayer*>(getParent()))
    {
        if (Multi::ROLE_STATUS == Multi::Role::None){
            getParent()->removeChild(this);
            log("UnitSprite deleted");
        }
        if (Multi::Role::Client ==  Multi::ROLE_STATUS)
        {
            // TODO: Show visual effect and ....
            std::string data = "";
            gameLayer->getMulti()->emit("", Value(data));
        }
        else{
            // TODO: host
        }
    }
}

