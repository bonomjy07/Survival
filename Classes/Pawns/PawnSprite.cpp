//
//  Pawn.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/05.
//

#include "PawnSprite.h"
#include "TestScene.h"

USING_NS_CC;

PawnSprite* PawnSprite::create(const std::string& filename, const float initialHealth)
{
    PawnSprite* sprite = new (std::nothrow) PawnSprite(initialHealth);
    if (sprite && sprite->initWithFile(filename) && sprite->initPhysics())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PawnSprite::PawnSprite(float initialHealth) : _currentHealth(initialHealth), _deltaPosition(0.f, 0.f), _interactableObject(nullptr)
{
    log("PawnSprite is created");
    
    static int id = 0;
    setName("pawn::" + std::to_string(++id));
}

PawnSprite::~PawnSprite()
{
    log("PawnSprite is deleted");
}

void PawnSprite::setCurrentHealth(float newHealth)
{
    _currentHealth = newHealth;
}

void PawnSprite::setCurrentDirection(const PawnDirection& newDirection)
{
    _currentDirection = newDirection;
}

float PawnSprite::getCurrentHealth() const
{
    return _currentHealth;
}

const PawnDirection& PawnSprite::getCurrentDirection() const
{
    return _currentDirection;
}

Vec2 PawnSprite::getDeltaPositionOnDirection() const
{
    // Diagnal movement is not allowed
    if (fabs(_deltaPosition.x) == fabs(_deltaPosition.y))
    {
        if (_currentDirection == PawnDirection::Up ||
            _currentDirection == PawnDirection::Down)
        {
            return Vec2(0, _deltaPosition.y);
        }
        else if (_currentDirection == PawnDirection::Right ||
                 _currentDirection == PawnDirection::Left)
        {
            return Vec2(_deltaPosition.x, 0);
        }
    }
    
    // If it's not diagnal, just return delta position
    return _deltaPosition;
}

const Vec2& PawnSprite::getDeltaPosition() const
{
    return _deltaPosition;
}

void PawnSprite::takeDamage(float damagedHealth)
{
    setCurrentHealth(_currentHealth - damagedHealth);
}

void PawnSprite::addDeltaPosition(float x, float y)
{
    _deltaPosition.x += x;
    _deltaPosition.y += y;
}

bool PawnSprite::initPhysics()
{
    PhysicsBody* pBody;
    if (!(pBody = PhysicsBody::createBox(Size(28.f, 28.f), PhysicsMaterial(0.1f, 1.0f, 0.0f))))
    {
        log("Failed to create physcis body for pawn");
        return false;
    }
    pBody->setGroup(0x01);
    pBody->setCategoryBitmask(0x02);
    pBody->setCollisionBitmask(0x04);
    pBody->setContactTestBitmask(0x08);
    this->addComponent(pBody);
    return true;
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

bool PawnSprite::canPawnMove(const Vec2& newPosition)
{
    // 1. Only one action is allowed at a time
    if (getNumberOfRunningActions() < 1)
    {
        // 2. Pawn needs gameLayer for tiled Map information
        if (auto gameLayer = TestScene::getGameLayer())
        {
            // 3. New position must be within the world
            const auto map = gameLayer->getTiledMap();
            if (0.f <= newPosition.x && newPosition.x <= map->getTileSize().width*map->getMapSize().width &&
                0.f <= newPosition.y && newPosition.y <= map->getTileSize().height*map->getMapSize().height)
            {
                // 4. A tile ahead of player mustn't be collidable tile
                if (!gameLayer->isCollidableTileForPosition(newPosition))
                {
                    // 5. A object ahead of player mustn't exist
                    float distance = map->getTileSize().width;
                    Vec2 actualPosition = getPosition() + gameLayer->getPosition(); // Real pawn's position
                    if (!gameLayer->checkNodeAtPosition(actualPosition+(getFrontVec2()*distance)))
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

Node* PawnSprite::checkFrontObject(float distance)
{
    Node* node = nullptr;
    auto currentScene = Director::getInstance()->getRunningScene();
    if (auto pWorld = currentScene->getPhysicsWorld())
    {
        if (auto gameLayer = static_cast<TestScene*> (currentScene->getChildByName("GameLayer")))
        {
            Vec2 fv = getFrontVec2()*distance;
            Vec2 point(this->getPositionX() + gameLayer->getPositionX() + fv.x, this->getPositionY() + gameLayer->getPositionY() + fv.y);
            pWorld->queryPoint(CC_CALLBACK_3(PawnSprite::OnQueryPoint, this), point, (void*)&node);
        }
    }
    return node;
}

bool PawnSprite::OnQueryPoint(PhysicsWorld& world, PhysicsShape& shape, void* data)
{
    if (auto pBody = shape.getBody())
    {
        // 'data' is holding address of node to be returned
        *static_cast<Node**>(data) = pBody->getNode();
    }
    return true;
}

Vec2 PawnSprite::getFrontVec2() const
{
    Vec2 v(0.f, 0.f);
    if (_currentDirection == PawnDirection::Up)
    {
        v = {0.f, +1.f};
    }
    else if (_currentDirection == PawnDirection::Down)
    {
        v = {0.f, -1.f};
    }
    else if (_currentDirection == PawnDirection::Right)
    {
        v = {+1.f, 0.f};
    }
    else if (_currentDirection == PawnDirection::Left)
    {
        v = {-1.f, 0.f};
    }
    return v;
}
