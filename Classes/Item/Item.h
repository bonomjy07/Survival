#ifndef ITEM_H__
#define ITEM_H__

#include "cocos2d.h"

class Item
{
public:
    Item();
    virtual ~Item();

    const char * getDescription();

    const char * getImageFileName();
    const char * getThumbnailFileName();

    virtual const bool isMaterial() override;
    virtual bool use() override;

protected:
    // description of item
    const char * &description;

    // item image on tile
    const char * &imagefileName;
    // item image on ui
    const char * &thumbnailFileName;

    int weight;
};

// craft item interface
class Craftable
{
    virtual ~Craftable();

    virtual bool craft() override;
};

// consume item interface
class Consumable
{
    virtual ~Consumable();

    virtual bool consume() override;
};

// place item interface
class Placeable
{
    virtual ~Placeable();

    virtual bool place() override;
};

// ride item interface
class Rideable
{
    virtual ~Rideable();

    virtual bool ride() override;
};

// swing item interface
class Swingable
{
    virtual ~Swingable();

    virtual bool swing() override;
};

#endif // ITEM_H__