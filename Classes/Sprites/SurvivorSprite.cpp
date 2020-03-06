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
#include "Tool.h"

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
    setPositionItemOnHand();
}

SurvivorSprite::SurvivorSprite(float health) : PawnSprite(health), _stat(), _itemOnMyTile(nullptr), _itemOnHand (nullptr)
{
    // Set base value for drain delay
    _drainDelay = 1.f;
    
    // Create input controller
    _inputController = new InputController();
    // Set input-action up
    setupInputAction();

    // Register contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactSeparate = CC_CALLBACK_1(SurvivorSprite::onContactSeparate, this);
    contactListener->onContactBegin = CC_CALLBACK_1(SurvivorSprite::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
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
                collectAction(itemSprite);
                emitDoAction("Collect", itemSprite->getName());
            }
        }
    }
    
}

void SurvivorSprite::collectAction(ItemSprite *itemSprite){
    // TODO : collect effect of SurvivorSprite
    if (auto gameLayer = dynamic_cast<GameLayer*>(this->_parent))
    {
        inventory.pushBack(itemSprite);
        // TODO: Implement inventory member variable.....
        
        itemSprite->wasCollected(); // Show visual effect and delete ItemSprite on gameLayer
        
        gameLayer->removeChild(itemSprite);
        if ( dynamic_cast<Sword*>(itemSprite->getItem()) ) {
            setItemOnHand(itemSprite);
        }
    }
}

void SurvivorSprite::useItemOnHand(){
    if (!_itemOnHand)
        return;
    if (auto gameLayer = dynamic_cast<GameLayer*>(this->_parent))
    {
        // Get nodes at the sprite's position
        Vector<Node*> nodes;
        Vec2 frontVec2 = getFrontVec2();
        
        gameLayer->checkNodesAtPosition(getPosition()+(frontVec2*32.f), &nodes);
        for (const auto& node : nodes)
        {
            if (ItemSprite* itemSprite = dynamic_cast<ItemSprite*>(node)){
                continue;
            }
            else if ( UnitSprite* unit = static_cast<UnitSprite*>(node) ) {
                useAction(_itemOnHand, unit);
                emitDoAction("Use", _itemOnHand->getName(), unit->getName());
            }
        }
    }
}

void SurvivorSprite::useAction(ItemSprite *itemSprite, UnitSprite *toUnit){
    // TODO : use effect of SurvivorSprite
    itemSprite->wasUsed(toUnit);
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
        _inputController->onPressed = BIND_ACTION(SurvivorSprite::onPressed);
        _inputController->onReleased = BIND_ACTION(SurvivorSprite::onReleased);
    }
}

void SurvivorSprite::onPressed(std::string action, InputController::InputEvent inputevent){
    Vec2 position = {0,0};
    if (!action.compare("Up")) {
        addDeltaPosition(0.f, +32.f);
        insertDirection(Direction::Up);
    }
    else if (!action.compare("Down")) {
        addDeltaPosition(0.f, -32.f);
        insertDirection(Direction::Down);
    }
    else if (!action.compare("Right")) {
        addDeltaPosition(+32.f, 0.f);
        insertDirection(Direction::Right);
    }
    else if (!action.compare("Left")) {
        addDeltaPosition(-32.f, 0.f);
        insertDirection(Direction::Left);
    }
    else if (!action.compare("Collect")) {
        collect();
    }
    else if (!action.compare("Use")) {
        useItemOnHand();
    }
}

void SurvivorSprite::onReleased(std::string action, InputController::InputEvent inputevent){
    if (!action.compare("Up")) {
        addDeltaPosition(0.f, -32.f);
        eraseDirection(Direction::Up);
    }
    else if (!action.compare("Down")) {
        addDeltaPosition(0.f, +32.f);
        eraseDirection(Direction::Down);
    }
    else if (!action.compare("Right")) {
        addDeltaPosition(-32.f, 0.f);
        eraseDirection(Direction::Right);
    }
    else if (!action.compare("Left")) {
        addDeltaPosition(+32.f, 0.f);
        eraseDirection(Direction::Left);
    }
}

void SurvivorSprite::emitDoAction(const std::string action, const std::string itemID){
    emitDoAction(action, itemID, "");
}
void SurvivorSprite::emitDoAction(const std::string action, const std::string itemID, const std::string toUnitID){
    if (Multi::ROLE_STATUS == Multi::Role::Host){

        if (auto gameLayer = dynamic_cast<GameLayer*>(getParent())){
            auto multi = gameLayer->getMulti();
            ValueMap data = ValueMap();
            data["ID"] = getName();
            data["itemID"] = itemID;
            data["toUnitID"] = toUnitID;
            data["action"] = action;

            multi->emit("doAction", data);
        }
    }
}
void SurvivorSprite::doAction(const std::string action, const std::string itemID, std::string toUnitID){
    // TODO : need MySprite Manager, find item and toUnit, doAction()
    // doAction(action, itemSprite, toUnit);
}
void SurvivorSprite::doAction(const std::string action, ItemSprite *itemSprite, UnitSprite *toUnit){
    if (Multi::ROLE_STATUS == Multi::Role::Client){
        if (!action.compare("Collect")) {
            collectAction(itemSprite);
        }
        else if (!action.compare("Use")) {
            useAction(itemSprite, toUnit);
        }
    }
}

void SurvivorSprite::setPositionItemOnHand(){
    if ( !_itemOnHand )
        return;
    
    Vec2 position = this->getPosition();
    _itemOnHand->setPosition(position.x+16.f, position.y+16.f);
}

void SurvivorSprite::setItemOnHand(ItemSprite* itemSprite){
    if (_itemOnHand)
        delete _itemOnHand;
    _itemOnHand = ItemSprite::create();
    _itemOnHand->setItem(itemSprite   ->getItem());
    _parent->addChild(_itemOnHand);
    setPositionItemOnHand();
}

bool SurvivorSprite::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    ItemSprite* item;
    if ((item = dynamic_cast<ItemSprite*>(nodeA)))
    {
        _itemOnMyTile = item;
    }
    else if ((item = dynamic_cast<ItemSprite*>(nodeB)))
    {
        _itemOnMyTile = item;
    }
    
    return true;
}

bool SurvivorSprite::onContactSeparate(cocos2d::PhysicsContact& contact)
{
    _itemOnMyTile = nullptr;
    return true;
}
