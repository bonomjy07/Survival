
#include "ItemSprite.h"
#include "UnitSprite.h"
#include "Food.h"
#include "Tool.h"

USING_NS_CC;

ItemSprite::ItemSprite(){}

ItemSprite::~ItemSprite()
{
    if( _item )
        _item->release();
}

bool ItemSprite::init()
{
    if(!Sprite::init()){
        return false;
    }
    _item = nullptr;
    return true;
}
bool ItemSprite::init(const std::string &itemtype)
{
    if(!init()){
        return false;
    }
    Item *item = makeItemByType(itemtype);
    setItem(item);
    return true;
}
bool ItemSprite::init(Item *item)
{
    if(!init()){
        return false;
    }
    setItem(item);
    return true;
}

Item * ItemSprite::getItem()
{
    return this->_item;
}

void ItemSprite::setItem(Item *item)
{
    if( this->_item )
        this->_item->release();
    item->retain();
    this->_item = item;
    this->setItemTexture();
}

void ItemSprite::setItemTexture()
{
    if(this->_item){
        this->setTexture(this->_item->getImageFileName());
        setContentSize(Size(32,32));
    }
}

void ItemSprite::wasCollected()
{
    removeFromParent();
    log("ItemSprite: Item %s was collected", _item->getName());
}

void ItemSprite::wasUsed(UnitSprite *toUnit)
{
    _item->use(toUnit);
    log("ItemSprite: Item %s was used", _item->getName());
}

Item *ItemSprite::makeItemByType(const std::string &itemtype){
    Item *item = nullptr;
    if (!itemtype.compare("DeerMeat")){
        item = DeerMeat::create();
    }
    else if (!itemtype.compare("Sword")){
        item = Sword::create();
    }
    return item;
}