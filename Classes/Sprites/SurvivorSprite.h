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
    
    void startDrainStats();
    
    void stopDrainStats();
    
    void collect();
    //void useItemOnHand();
    
private:
    Stat _stat;
    float _drainDelay;
    class Item* _itemOnHand;
     // cocos2d::Vector<class Item*> inventory;
    // class ItemSprite* _collectable;
    
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
    
};
#endif /* SurvivorSprite_h */
