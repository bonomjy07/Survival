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
#include "InputController.h"

USING_NS_CC;

SurvivorSprite* SurvivorSprite::create(const std::string &filename, float maxHealth)
{
    SurvivorSprite* sprite = new (std::nothrow) SurvivorSprite(maxHealth);
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
}

SurvivorSprite::SurvivorSprite(float health) : PawnSprite(health), _stat()
{
    // Set base value for drain delay
    _drainDelay = 1.f;
    
    // Create input controller
    _inputController = new InputController();
    // Set input-action up
    setupInputAction();
}

SurvivorSprite::~SurvivorSprite()
{
    inventory.clear();
    delete _inputController;
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
               log("item...");
               
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

InputController* SurvivorSprite::getInputController() const
{
    return _inputController;
}

void SurvivorSprite::setupInputAction()
{
    void* arg = nullptr;
    
    if (_inputController)
    {
        _inputController->bindAction("Up", InputController::InputEvent::KeyPressed, std::bind(&SurvivorSprite::movePressedUp, this, arg));
        _inputController->bindAction("Down", InputController::InputEvent::KeyPressed, std::bind(&SurvivorSprite::movePressedDown, this, arg));
        _inputController->bindAction("Right", InputController::InputEvent::KeyPressed, std::bind(&SurvivorSprite::movePressedRight, this, arg));
        _inputController->bindAction("Left", InputController::InputEvent::KeyPressed, std::bind(&SurvivorSprite::movePressedLeft, this, arg));
        
        _inputController->bindAction("Up", InputController::InputEvent::KeyReleased, std::bind(&SurvivorSprite::moveReleasedUp, this, arg));
        _inputController->bindAction("Down", InputController::InputEvent::KeyReleased, std::bind(&SurvivorSprite::moveReleasedDown, this, arg));
        _inputController->bindAction("Right", InputController::InputEvent::KeyReleased, std::bind(&SurvivorSprite::moveReleasedRight, this, arg));
        _inputController->bindAction("Left", InputController::InputEvent::KeyReleased, std::bind(&SurvivorSprite::moveReleasedLeft, this, arg));
    }
}

// //////////////////////////////////////////
void SurvivorSprite::movePressedUp(void *arg)
{
    addDeltaPosition(0.f, +32.f);
    insertDirection(Direction::Up);
}

void SurvivorSprite::movePressedDown(void *arg)
{
    addDeltaPosition(0.f, -32.f);
    insertDirection(Direction::Down);
}

void SurvivorSprite::movePressedRight(void *arg)
{
    addDeltaPosition(+32.f, 0.f);
    insertDirection(Direction::Right);
}

void SurvivorSprite::movePressedLeft(void *arg)
{
    addDeltaPosition(-32.f, 0.f);
    insertDirection(Direction::Left);
}

// //////////////////////////////////////////
void SurvivorSprite::moveReleasedUp(void *arg)
{
    addDeltaPosition(0.f, -32.f);
    eraseDirection(Direction::Up);
}

void SurvivorSprite::moveReleasedDown(void *arg)
{
    addDeltaPosition(0.f, +32.f);
    eraseDirection(Direction::Down);
}

void SurvivorSprite::moveReleasedRight(void *arg)
{
    addDeltaPosition(-32.f, 0.f);
    eraseDirection(Direction::Right);
}

void SurvivorSprite::moveReleasedLeft(void *arg)
{
    addDeltaPosition(+32.f, 0.f);
    eraseDirection(Direction::Left);
}
