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
    void use();
    
private:
    Stat _stat;
    float _drainDelay;
    cocos2d::Vector<class Item*> inventory;
    // class ItemSprite* _collectable;
    
private:
    void drainStats(float dt);
};
#endif /* SurvivorSprite_h */
