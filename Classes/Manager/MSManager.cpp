
#include "MSManager.h"
#include "ItemSprite.h"
#include "UnitSprite.h"
#include "PawnSprite.h"
#include "SurvivorSprite.h"
#include "cocos2d.h"

USING_NS_CC;

std::map<std::string, std::pair<MSManager::Type, MySprite *>> MSManager::_manager;

MSManager::Type MSManager::getType(std::string id){
    auto it = _manager.find(id);
    MySprite *my = nullptr;
    if (it != _manager.end()){
        return (*it).second.first;
    }
}

bool MSManager::hasMySprite(std::string id){
    if (_manager.find(id) != _manager.end())
        return true;
    else
        return false;
}

void MSManager::addMySprite(MySprite *my){
    Type type;
    if (!dynamic_cast<MySprite *>(my)){
        type = MSManager::Type::MY_SPRITE;
    }
    else if (!dynamic_cast<ItemSprite *>(my)){
        type = MSManager::Type::ITEM_SPRITE;
    }
    else if (!dynamic_cast<UnitSprite *>(my)){
        type = MSManager::Type::UNIT_SPRITE;
    }
    else if (!dynamic_cast<PawnSprite *>(my)){
        type = MSManager::Type::PAWN_SPRITE;
    }
    else if (!dynamic_cast<SurvivorSprite *>(my)){
        type = MSManager::Type::SURVIVOR_SPRITE;
    }
    auto pair = std::pair<MSManager::Type, MySprite*>(type, my);
    cocos2d::log(my->getName().c_str());
    _manager.insert({my->getName(),pair});
}

MySprite *MSManager::getMySprite(std::string id){
    auto it = _manager.find(id);
    MySprite *my = nullptr;
    if (it != _manager.end()){
        my = (*it).second.second;
    }
    return my;
}

UnitSprite *MSManager::getAsUnit(std::string id){
    auto sprite = getMySprite(id);
    if (sprite)
        return dynamic_cast<UnitSprite *>(sprite);
    else
        return nullptr;
    
}

ItemSprite *MSManager::getAsItem(std::string id){
    auto sprite = getMySprite(id);
    if (sprite)
        return dynamic_cast<ItemSprite *>(sprite);
    else
        return nullptr;
    
}

PawnSprite *MSManager::getAsPawn(std::string id){
    auto sprite = getMySprite(id);
    if (sprite)
        return dynamic_cast<PawnSprite *>(sprite);
    else
        return nullptr;
    
}

SurvivorSprite *MSManager::getAsSurvivor(std::string id){
    auto sprite = getMySprite(id);
    if (sprite)
        return dynamic_cast<SurvivorSprite *>(sprite);
    else
        return nullptr;
    
}
void MSManager::remove(std::string id){
    _manager.erase(id);
}
