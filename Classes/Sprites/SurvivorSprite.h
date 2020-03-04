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
    cocos2d::Vector<class Item*> *getInventory(){ return &inventory; };
    void startDrainStats();
    
    void stopDrainStats();
    
    void collect();
    void useItemOnHand();
    void setItemOnHand(class ItemSprite*);
    void setPositionItemOnHand();
    
private:
    Stat _stat;
    float _drainDelay;
    class ItemSprite* _itemOnHand;
    cocos2d::Vector<class Item*> inventory;
    
private:
    void drainStats(float dt);
    
    /* Test Test Test
     * Test Test Test
     */
public:
    class InputController* getInputController() const;
    
private:
    class InputController* _inputController;
    
private:
    void setupInputAction();
    
private:
    void movePressedUp(void* arg);
    void movePressedDown(void* arg);
    void movePressedRight(void* arg);
    void movePressedLeft(void* arg);
    
    void moveReleasedUp(void* arg);
    void moveReleasedDown(void* arg);
    void moveReleasedRight(void* arg);
    void moveReleasedLeft(void* arg);
   
public:
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactSeparate(cocos2d::PhysicsContact& contact);
    class ItemSprite* _itemOnMe;
};
#endif /* SurvivorSprite_h */
