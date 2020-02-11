//
//  Pawn.h
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//
#pragma once

#ifndef Pawn_h
#define Pawn_h

#include "cocos2d.h"

#endif /* Pawn_h */

enum class PawnDirection
{
    Vertical,
    Horizon
};

class PawnSprite : public cocos2d::Sprite
{
public:
    static PawnSprite* create(const std::string& filename, const float initialHealth);
    PawnSprite(float initialHealth);
        
public:
    /**
     @brief Set current health newHealth
     */
    void setCurrentHealth(float newHealth);
    /**
     @brief Return current health
     */
    float getCurrentHealth() const;
    
    /**
     @brief Calculate amount of damage and set current heeath
     */
    void takeDamage(float damagedHealth);
        
    void setCurrentDirection(const PawnDirection& newDirection);
    const PawnDirection& getCurrentDirection() const;

    /**
     @brief Returns delta position on direction and
     never returns diagnol movement
     */
    void addDeltaPosition(float x, float y);
    cocos2d::Vec2 getDeltaPositionOnDirection() const;
    const cocos2d::Vec2& getDeltaPosition() const;
    
public:
    virtual void initPhysics();

protected:
    /**
     @brief Delta position is amount of movement
     Pawn moves as far as delta movement
     */
    cocos2d::Vec2 deltaPosition;
    /**
     @brief current direction is used not to allow diagnal movement
     */
    PawnDirection currentDirection;
    
    float currentHealth;

private:
    /* *Helper function for movement
     * This function return boolean
     * whether target postion is collidable */    
};
