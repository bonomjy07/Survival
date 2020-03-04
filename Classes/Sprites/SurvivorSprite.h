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
    cocos2d::Vector<class Item*> *getInventory(){ return &inventory; };
    
    void startDrainStats();
    void stopDrainStats();
    
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
    
public:
    class InputController* getInputController() const;
    
private:
    class InputController* _inputController;
    
private:
    void setupInputAction();
    
private:
    void onPressed(std::string action, InputController::InputEvent inputevent);
    void onReleased(std::string action, InputController::InputEvent inputevent);
    
    void collect();
   
private:
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactSeparate(cocos2d::PhysicsContact& contact);
    class ItemSprite* _itemOnMyTile;
};
#endif /* SurvivorSprite_h */
