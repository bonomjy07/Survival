//
//  SurvivorSprite.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "SurvivorSprite.h"
#include "TestScene.h"
#include "Item.h"
#include "ItemSprite.h"
#include "Tool.h"

USING_NS_CC;

SurvivorSprite* SurvivorSprite::create(const std::string &filename, float maxHealth)
{
    SurvivorSprite* sprite = new (std::nothrow) SurvivorSprite(maxHealth);
    // if (sprite && sprite->initWithFile(filename) && sprite->initPhysicsBody())
    if (sprite && sprite->initWithFile(filename) && sprite->initPhysicsBody())
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
    setPositionItemOnHand();
}

SurvivorSprite::SurvivorSprite(float health) : PawnSprite(health), _stat()
{
    _drainDelay = 1.f;
    _itemOnHand = nullptr;
}

SurvivorSprite::~SurvivorSprite()
{
    inventory.clear();
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

void SurvivorSprite::collect()
{
    if (auto gameLayer = dynamic_cast<GameLayer*>(this->_parent))
    {
        // Get nodes at the sprite's position
        Vector<Node*> nodes;
        gameLayer->checkNodesAtPosition(getPosition(), &nodes);
        for (const auto node : nodes)
        {
            if (ItemSprite* itemSprite = dynamic_cast<ItemSprite*>(node))
            {
                gameLayer->removeChild(itemSprite);
                Item *item = itemSprite->getItem();
                inventory.pushBack(item);
                // TODO: Implement inventory member variable.....
                
                // TODO: inventory.pushback(itemSprite->getItem());
                itemSprite->wasCollected(); // Show visual effect and delete ItemSprite on gameLayer
                // TODO: log("Item %s was collected", itemSprite->getName());
                if ( dynamic_cast<Sword*>(item) ) {
                    setItemOnHand(itemSprite);
                }
                
                // TODO: break; ??????
            }
        }
    }
}

void SurvivorSprite::drainStats(float dt)
{
    _stat.updateCurrentStamina(-1.f);
    _stat.updateCurrentThirsty(-2.f);
    _stat.updateCurrentHunger(-3.f);
    _stat.updateCurrentSleep(-4.f);
}

void SurvivorSprite::setPositionItemOnHand(){
    if ( !_itemOnHand )
        return;
    
    Vec2 position = this->getPosition();
    _itemOnHand->setPosition(position.x+16.f, position.y+16.f);
}

void SurvivorSprite::setItemOnHand(ItemSprite* itemSprite){
    _itemOnHand = itemSprite;
    _parent->addChild(_itemOnHand);
    setPositionItemOnHand();
}