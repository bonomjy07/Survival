//
//  SurvivorSprite.h
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//
#pragma once

#ifndef SurvivorSprite_h
#define SurvivorSprite_h

#include "cocos2d.h"

#include "PawnSprite.h"
#include "Stat.h"
#include "InputController.h"

class SurvivorSprite : public PawnSprite
{
public:
    static SurvivorSprite* create(const std::string& filename, float initialHealth);
    void update(float dt) override;

public:
    SurvivorSprite(float health);
    virtual ~SurvivorSprite();
    
public:
    const Stat& getStat() const;
    cocos2d::Vector<class ItemSprite*> *getInventory(){ return &inventory; };
    
    void startDrainStats();
    void stopDrainStats();

    void setItemOnHand(class ItemSprite *itemSprite);
    void setPositionItemOnHand();
    
private:
    Stat _stat;
    float _drainDelay;
    class ItemSprite* _itemOnHand;
    cocos2d::Vector<class ItemSprite*> inventory;
    
private:
    void drainStats(float dt);
    
public:
    /* Accessor function for input controller */
    class InputController* getInputController() const;
    
private:
    /* Input controller mananges functions corresponding actions */
    class InputController* _inputController;
    
private:
    /* Binds functions with action */
    void setupInputAction();
    
private:
    void onPressed(std::string action, InputController::InputEvent inputevent);
    void onReleased(std::string action, InputController::InputEvent inputevent);
    void emitDoAction(const std::string action, const std::string itemID, const std::string toUnitID="");
    
    /* Called when 'Up' action is pressed */
    void movePressed(Direction direction, void* arg);
    
    /* Called when 'Up' action is released */
    void moveReleased(Direction direction, void* arg);
    
    /* Called when 'Collecet' action is pressed */
    void collect(void *arg);
    void collectAction(void *arg);
   
    void useItemOnHand(void *arg);
    void useAction(void *itemSprite, void *toUnit);

private:
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactSeparate(cocos2d::PhysicsContact& contact);
    class ItemSprite* _itemOnMyTile;
};
#endif /* SurvivorSprite_h */
