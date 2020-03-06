//
//  Pawn.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "PawnSprite.h"
#include "TestScene.h"
#include "Multi.h"

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
            if (auto gameLayer = dynamic_cast<GameLayer*>(_parent))
            {
                gameLayer->getOccupied().erase(getPosition());
                gameLayer->getOccupied().insert(newPosition);
                moveThePawn(newPosition);
            }
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

Vec2 PawnSprite::getFrontVec2() const
{
    return getDirectionVec2(_currentDirection);
}

Vec2 PawnSprite::getDirectionVec2(const Direction &direction) const {
    Vec2 vec = Vec2::ZERO;
    switch(direction){
        case Direction::Up:
            vec =  {0.f, +1.f};
            break;
        case Direction::Down:
            vec = {0.f, -1.f};
            break;
        case Direction::Right:
            vec = {+1.f, 0.f};
            break;
        case Direction::Left:
            vec = {-1.f, 0.f};
            break;
    }
    return vec;
}

void PawnSprite::addDeltaPosition(float x, float y)
{
    _deltaPosition.x += x;
    _deltaPosition.y += y;
}
void PawnSprite::addDeltaPosition(Vec2 position)
{
    _deltaPosition += position;
}

void PawnSprite::insertDirection(Direction newDirection)
{
    setCurrentDirection(newDirection);
    _directionList.push_front(newDirection); // Recenlty pressed direction is at 'Begin'
}

void PawnSprite::eraseDirection(Direction releasedDirection)
{
    _directionList.remove(releasedDirection);
    // If multiple key pressed, use recent direction
    if (_directionList.size() > 0)
    {
        setCurrentDirection(*_directionList.begin());
    }
}

bool PawnSprite::canPawnMove(const Vec2& newPosition)
{
    // 1. Only one action is allowed at a time
    if (getNumberOfRunningActions() < 1)
    {
        // 2. Pawn needs gameLayer for world map information
        if (auto gameLayer = dynamic_cast<GameLayer*>(this->_parent))
        {
            // 3. New position must be within the world
            if (gameLayer->isPositionWithinWorld(newPosition))
            {
                // 4. New position mustn't be collidable tile
                if (!gameLayer->isCollidableTileForPosition(newPosition))
                {
                    // 5. No any unit at front && not occupied position(network sync)
                    Node* node = gameLayer->checkNodeAtPosition(getPosition(), getFrontVec2());
                    UnitSprite* unit = dynamic_cast<UnitSprite*>(node);
                    //auto occupied = gameLayer->getOccupied();
                   // if (!unit && occupied.find(newPosition) == occupied.end())
                    if (!unit )
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
    if (auto gameLayer = dynamic_cast<GameLayer*>(getParent()))
    {
        float duration = 0.125f; // Less duration, More speed
        auto moveTo = MoveTo::create(duration, newPosition);
        runAction(moveTo);
        
        // If multiplayer game, broadcasts pawn's movement to clients
        if (Multi::ROLE_STATUS == Multi::Role::Host){
            auto multi = dynamic_cast<Multi*>(gameLayer->getChildByName("MultiGame"));
            ValueMap data = ValueMap();
            data["ID"] = getName();
            data["x"] = newPosition.x;
            data["y"] = newPosition.y;

            multi->emit("pawnMove", data);
        }
    }
}
