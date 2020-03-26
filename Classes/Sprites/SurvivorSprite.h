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
    /* Binds functions with actions */
    void setupInputAction();
    
private:
    void emitDoAction(const std::string action, const std::string itemID, const std::string toUnitID="");
    
    /* Called when 'Up','Down', 'Right', 'Left" action are pressed */
    void startMove(Direction direction, void* arg);
    
    /* Called when 'Up','Down', 'Right', 'Left" action are released */
    void stopMove(Direction direction, void* arg);
    
    /* Called when 'Collecet' action is pressed */
    void collect(void *arg);
    void collectAction(void *arg);
   
    void useItemOnHand(void *arg);
    void useAction(void *itemSprite, void *toUnit);

private:
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactSeparate(cocos2d::PhysicsContact& contact);
    class ItemSprite* _itemOnMyTile;
    
private:
    /** Each direction animation are stored in walking frames */
    std::map<PawnSprite::Direction, const cocos2d::Vector<cocos2d::SpriteFrame*>> _walkingFrames;
    
    /** Store animation frame in  cocos2dx cache */
    void storeAnimationCache(const std::string& format);
    
    /** Return animation frames from resource files corresponding the format */
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimationFrames(const std::string& format);
    
    /** Run animation corresponding the direction */
    void runWalkingAnimation(PawnSprite::Direction direction);
    
    void onDirectionChange() override;
    
public:
    /** Added walking animations when the character walks */
    void moveThePawn(const cocos2d::Vec2& newPosition) override;
};
#endif /* SurvivorSprite_h */
