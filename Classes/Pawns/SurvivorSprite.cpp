//
//  SurvivorSprite.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "SurvivorSprite.h"
#include "TestScene.h"
#include "Item.h"

USING_NS_CC;

SurvivorSprite* SurvivorSprite::create(const std::string &filename, float initialHealth)
{
    SurvivorSprite* sprite = new (std::nothrow) SurvivorSprite(initialHealth);
    if (sprite && sprite->initWithFile(filename) && sprite->initPhysics())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void SurvivorSprite::update(float dt)
{
    PawnSprite::update(dt);
    
    // Drains the stats
}

SurvivorSprite::SurvivorSprite(float health) : PawnSprite(health), _stat()
{
    _drainDelay = 1.f;
}

SurvivorSprite::~SurvivorSprite()
{
}

const Stat& SurvivorSprite::getStat() const
{
    return _stat;
}

void SurvivorSprite::startDrainStats()
{
    SEL_SCHEDULE selector = schedule_selector(SurvivorSprite::drainStats);
    if (!this->isScheduled(selector))
    {
        this->schedule(selector, _drainDelay);
    }
}

void SurvivorSprite::stopDrainStats()
{
    SEL_SCHEDULE selector = schedule_selector(SurvivorSprite::drainStats);
    if (this->isScheduled(selector))
    {
        this->unschedule(selector);
    }
}

// TODO: Item must has physicsBody.....
void SurvivorSprite::collect()
{
    // Get a node at front
    if (auto gameLayer = TestScene::getGameLayer())
    {
        // TODO: Casting ItemSprite later....
        if (Node* node = gameLayer->checkNodeAtPosition(getPosition(), getFrontVec2()))
        {
            // TODO: Add it inventory
            // TODO: item->wasCollected();
        }
    }
}

void SurvivorSprite::drainStats(float dt)
{
    _stat.updateCurrentThirsty(-1.f);
    _stat.updateCurrentHunger(-1.f);
    _stat.updateCurrentThirsty(-1.f);
    _stat.updateCurrentStamina(-1.f);
}
