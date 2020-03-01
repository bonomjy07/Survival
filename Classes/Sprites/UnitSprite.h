//
//  UnitSprite.h
//  Survival
//
//  Created by jy_maeng on 2020/02/18.
//

#pragma once

#ifndef UnitSprite_h
#define UnitSprite_h

#include "MySprite.h"
#include "ui/CocosGUI.h"

#define MAX_HEALTH 100.f

class UnitSprite : public MySprite
{
public:
    static UnitSprite* create(const std::string& filename, float maxHealth = MAX_HEALTH);

public:
    UnitSprite(float maxHealth = MAX_HEALTH);
    virtual ~UnitSprite();
    
public:
    /* Returns unit's max health */
    float getMaxHealth() const;
    
    /* Returns unit's current health */
    float getCurrentHealth() const;
    
    /* Set current health newCurrentHealth */
    void setCurrentHealth(float newCurrnetHealth);
    
    /**
     @Brief Takes the damages unit gets
     @details Delta damage will be added to current health
     */
    void takeDamage(float deltaDamage);
    
protected:
    float _currentHealth;
    float _maxHealth;
    
protected:
    virtual void onDeath();
};

#endif /* UnitSprite_h */
