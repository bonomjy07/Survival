
#include "ItemSprite.h"
#include "UnitSprite.h"
USING_NS_CC;

ItemSprite::ItemSprite(){}

ItemSprite::~ItemSprite()
{
    if( item )
        item->release();
}

bool ItemSprite::init()
{
    if(!Sprite::init()){
        return false;
    }
    item = nullptr;
    return true;
}

Item * ItemSprite::getItem()
{
    return this->item;
}

void ItemSprite::setItem(Item *_item)
{
    if( this->item )
        this->item->release();
    _item->retain();
    this->item = _item;
    this->setItemTexture();
}

void ItemSprite::setItemTexture()
{
    if(this->item){
        this->setTexture(this->item->getImageFileName());
        setContentSize(Size(32,32));
    }
}

void ItemSprite::wasCollected()
{
    log("ItemSprite: Item %s was collected", item->getName());
}

void ItemSprite::wasUsed(UnitSprite *toUnit)
{
    item->use(toUnit);
    log("ItemSprite: Item %s was used", item->getName());
}