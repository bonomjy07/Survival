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

enum class PawnDirection
{
    Vertical,
    Horizon,
    Up = 1, Down, Right, Left
};

class PawnSprite : public cocos2d::Sprite
{
public:
    static PawnSprite* create(const std::string& filename, const float initialHealth);
    void update(float dt) override;

public:
    PawnSprite(float initialHealth);
    virtual ~PawnSprite();
    
public:
    /**
     @brief Set current health newHealth
     */
    void setCurrentHealth(float newHealth);

    /**
    @brief Updates direction on new direction
    */
    void setCurrentDirection(const PawnDirection& newDirection);

    /**
    @brief Returns pawn's current health
    */
    float getCurrentHealth() const;

    /**
    @brief Returns pawn's current direction
    */
    const PawnDirection& getCurrentDirection() const;

    /**
    @brief Returns pawn's delta position, that doesn't stand diagonal movement
    @details Return values ignore 'X' or 'Y' on current direction
    */
    cocos2d::Vec2 getDeltaPositionOnDirection() const;

    /**
    @brief Returns pawn's delta position as it is
    */
    const cocos2d::Vec2& getDeltaPosition() const;
    
    /**
     @brief Retruns front vector for pawn
     */
    cocos2d::Vec2 getFrontVec2() const;
    
    /**
     @brief Calculate amount of damage and set current heeath
     */
    void takeDamage(float damagedHealth);
    
    /**
     @brief Adds delta position
     */
    void addDeltaPosition(float x, float y);
    
protected:
    /**
     @brief Delta position is amount of movement
     Pawn moves as far as delta movement
     */
    cocos2d::Vec2 _deltaPosition;
    
    /**
     @brief current direction is used not to allow diagnal movement
     */
    PawnDirection _currentDirection;
    
    /** Current health of the pawn */
    float _currentHealth;
    
protected:
    /**
     @brief Initiates physics for pawn, this is called in the constructor.
     Returns false if it fails to create physics box.
     @TODO Hard-coding is needed to be removed...
     */
    bool initPhysics();
    
    /**
     @brief Returns true if new position is valid position to move
     */
    bool canPawnMove(const cocos2d::Vec2& newPosition);
    
    /**
     @brief Called every frame if delta position is not zero.
     Moves the pawn to new position
     @warning CanPawnMove function must be called before call this function
     */
    void moveThePawn(const cocos2d::Vec2& newPosition);
};

#endif /* Pawn_h */

