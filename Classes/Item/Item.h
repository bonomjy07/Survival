#ifndef ITEM_H__
#define ITEM_H__

#include "cocos2d.h"
#include "UnitSprite.h"

class Item : public cocos2d::Ref
{
public:
    Item();
    Item(const char *name, const char *des, const char * img,
    const char * thumb, const int weight);
    virtual ~Item();

    const char * getName(){ return _name; }
    const char * getDescription(){ return _description; }
    const char * getImageFileName(){return _imageFileName; }
    const char * getThumbnailFileName(){ return _thumbnailFileName; }
    const int getWeight(){ return _weight; }

    virtual bool isMaterial()=0;
    virtual bool use(UnitSprite *toUnit)=0;

protected:
    // name of item
    const char * const _name;

    // description of item
    const char * const _description;

    // item image on tile
    const char * const _imageFileName;
    // item image on ui
    const char * const _thumbnailFileName;

    const int _weight;
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
