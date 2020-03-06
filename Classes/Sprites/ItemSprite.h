#ifndef ITEM_SPRITE_H__
#define ITEM_SPRITE_H__

#include "Item.h"
#include "MySprite.h"

class ItemSprite : public MySprite
{
public:
    ItemSprite();
    virtual ~ItemSprite();
    virtual bool init();
    CREATE_FUNC(ItemSprite);

    Item * getItem();
    void setItem(Item *_item);

    void setItemTexture();
    void wasCollected();
    void wasUsed(class UnitSprite*);
protected:
    Item *item;
};

#endif //ITEM_SPRITE_H__