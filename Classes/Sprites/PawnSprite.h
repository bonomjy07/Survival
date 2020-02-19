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
#include "UnitSprite.h"

class PawnSprite : public UnitSprite
{
public:
    static PawnSprite* create(const std::string& filename, const float maxHealth);
    void update(float dt) override;
    
    enum class Direction { Up = 0, Down, Right, Left };

public:
    PawnSprite(float maxHealth);
    virtual ~PawnSprite();
    
public:
    /**
     @brief Updates direction on new direction
     */
    void setCurrentDirection(const PawnSprite::Direction& newDirection);
    
    /**
     @brief Returns pawn's current direction
     */
    const Direction& getCurrentDirection() const;
    
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
    Direction _currentDirection;
    
protected:
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

