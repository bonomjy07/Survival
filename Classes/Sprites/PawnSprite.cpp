//
//  Pawn.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "PawnSprite.h"
#include "TestScene.h"

USING_NS_CC;

PawnSprite* PawnSprite::create(const std::string& filename, const float maxHealth)
{
    PawnSprite* sprite = new (std::nothrow) PawnSprite(maxHealth);
    if (sprite && sprite->initWithFile(filename) && sprite->initPhysicsBody())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void PawnSprite::update(float dt)
{
    if (!_deltaPosition.equals(Vec2::ZERO))
    {
        Vec2 newPosition = getPosition()+getDeltaPositionOnDirection();
        if (canPawnMove(newPosition))
        {
            moveThePawn(newPosition);
        }
    }
}

PawnSprite::PawnSprite(float maxHealth) : UnitSprite(maxHealth), _deltaPosition(0.f, 0.f) {}

PawnSprite::~PawnSprite() {}

void PawnSprite::setCurrentDirection(const Direction& newDirection)
{
    _currentDirection = newDirection;
}

const PawnSprite::Direction& PawnSprite::getCurrentDirection() const
{
    return _currentDirection;
}

Vec2 PawnSprite::getDeltaPositionOnDirection() const
{
    // Diagnal movement is not allowed
    if (fabs(_deltaPosition.x) == fabs(_deltaPosition.y))
    {
        if ((Direction::Up == _currentDirection) ||
            (Direction::Down == _currentDirection))
        {
            return Vec2(0.f, _deltaPosition.y);
        }
        else if ((Direction::Right == _currentDirection) ||
                 (Direction::Left == _currentDirection))
        {
            return Vec2(_deltaPosition.x, 0.f);
        }
    }
    
    // If it's not diagnal, just return delta position
    return _deltaPosition;
}

const Vec2& PawnSprite::getDeltaPosition() const
{
    return _deltaPosition;
}

Vec2 PawnSprite::getFrontVec2() const
{
    if (Direction::Up == _currentDirection)
    {
        return {0.f, +1.f};
    }
    else if (Direction::Down == _currentDirection)
    {
        return {0.f, -1.f};
    }
    else if (Direction::Right == _currentDirection)
    {
        return {+1.f, 0.f};
    }
    else if (Direction::Left == _currentDirection)
    {
        return {-1.f, 0.f};
    }
    return Vec2::ZERO;
}

void PawnSprite::addDeltaPosition(float x, float y)
{
    _deltaPosition.x += x;
    _deltaPosition.y += y;
}

bool PawnSprite::canPawnMove(const Vec2& newPosition)
{
    // 1. Only one action is allowed at a time
    if (getNumberOfRunningActions() < 1)
    {
        // 2. Pawn needs gameLayer for tiled Map information
        if (auto gameLayer = TestScene::getGameLayer())
        {
            // 3. New position must be within the world
            Vec2 newPosition = getPosition() + getDeltaPositionOnDirection();
            if (gameLayer->isPositionWithinWorld(newPosition))
            {
                // 4. New position mustn't be collidable tile
                if (!gameLayer->isCollidableTileForPosition(newPosition))
                {
                    // 5. A sprite ahead of player mustn't be unit-sprite
                    Node* node = gameLayer->checkNodeAtPosition(getPosition(), getFrontVec2());
                    UnitSprite* unit = dynamic_cast<UnitSprite*>(node);
                    if (!unit)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void PawnSprite::moveThePawn(const Vec2 &newPosition)
{
    float duration = 0.125f; // Less duration, More speed
    auto moveTo = MoveTo::create(duration, newPosition);
    this->runAction(moveTo);
}
