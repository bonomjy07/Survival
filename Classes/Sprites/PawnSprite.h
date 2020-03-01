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
    
public:
    enum class Direction { Up = 0, Down, Right, Left };

public:
    PawnSprite(float maxHealth);
    virtual ~PawnSprite();
    
public:
    /* Updates direction on new direction */
    void setCurrentDirection(const PawnSprite::Direction& newDirection);
    
    /* Returns pawn's current direction */
    const Direction& getCurrentDirection() const;
    
    /**
     @brief Returns pawn's delta position, that doesn't stand diagonal movement
     @details Return values ignore 'X' or 'Y' on current direction
     */
    cocos2d::Vec2 getDeltaPositionOnDirection() const;
    
     /* Returns pawn's delta position as it is */
    const cocos2d::Vec2& getDeltaPosition() const;
    
     /* Retruns front vector for pawn */
    cocos2d::Vec2 getFrontVec2() const;
    
    /**
     @brief Adds delta position
     */
    void addDeltaPosition(float x, float y);
    
    /**
     @brief Inserts new direction at begin of _directionList
     */
    void insertDirection(Direction newDirection);
    
    /**
     @brief Erases released direction in _directionList
     @details If multi key were pressed, get recent direction in _list
     */
    void eraseDirection(Direction releasedDirection);
    
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
    /* Returns true if new position is valid position to move */
    bool canPawnMove(const cocos2d::Vec2& newPosition);
    
public:
    /**
     @brief Called every frame if delta position is not zero.
     Moves the pawn to new position
     @warning CanPawnMove function must be called before call this function
     */
    void moveThePawn(const cocos2d::Vec2& newPosition);
    
private:
    /* New direction has to be insert at begin(first) */
    std::list<Direction> _directionList;
    
};

#endif /* Pawn_h */

