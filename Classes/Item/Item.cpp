
#include "Item.h"

// def item class
Item::Item(const char * _des, const char * _img,
     const char * _thumb, const int _weight)
    :description(_des), imageFileName(_img),
     thumbnailFileName(_thumb), weight(_weight)
{

}

Item::~Item()
{

}

const char * Item::getDescription()
{
    return this->description;
}

const char * Item::getImageFileName()
{
    return this->imageFileName;
}

const char * Item::getThumbnailFileName()
{
    return this->thumbnailFileName;
}

// item class


// def ItemSprite class
ItemSprite::ItemSprite(){}

ItemSprite::~ItemSprite()
{
    item->release();
}

bool ItemSprite::init()
{
    if(!Sprite::init()){
        return false;
    }
    return true;
}

Item * ItemSprite::getItem()
{
    return this->item;
}
void ItemSprite::setItem(Item *_item)
{
    this->item = _item;
    this->setItemTexture();
}
void ItemSprite::setItemTexture(){
    if(this->item){
        this->setTexture(this->item->getImageFileName());
    }
}
// ItemSprite class
