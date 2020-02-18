#ifndef ITEM_H__
#define ITEM_H__

#include "cocos2d.h"

class Item
{
public:
    Item();
    Item(const char * _des, const char * _img,
     const char * _thumb, const int _weight);
    virtual ~Item();

    const char * getDescription();

    const char * getImageFileName();
    const char * getThumbnailFileName();

    virtual bool isMaterial()=0;
    virtual bool use()=0;

protected:
    // description of item
    const char * const description;

    // item image on tile
    const char * const imageFileName;
    // item image on ui
    const char * const thumbnailFileName;

    const int weight;
};

class ItemSprite : public cocos2d::Sprite
{
public:
    ItemSprite();
    virtual ~ItemSprite();

    virtual bool init();

    Item * getItem();
    void setItem(Item *_item);

    void setItemTexture();

    CREATE_FUNC(ItemSprite);
protected:
    Item *item;
};

// craft item interface
class Craftable
{
    virtual ~Craftable();

    virtual bool craft();
};

// consume item interface
class Consumable
{
    virtual ~Consumable();

    virtual bool consume();
};

// place item interface
class Placeable
{
    virtual ~Placeable();

    virtual bool place();
};

// ride item interface
class Rideable
{
    virtual ~Rideable();

    virtual bool ride();
};

// swing item interface
class Swingable
{
    virtual ~Swingable();

    virtual bool swing();
};

#endif // ITEM_H__
