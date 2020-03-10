#ifndef ITEM_SPRITE_H__
#define ITEM_SPRITE_H__

#include "Item.h"
#include "MySprite.h"
#include "Create.h"

class ItemSprite : public MySprite
{
public:
    ItemSprite();
    virtual ~ItemSprite();
    virtual bool init();
    virtual bool init(const std::string &itemtype);
    virtual bool init(Item *item);
    CREATE_FUNC(ItemSprite);
    CREATE_FUNC_1(ItemSprite, Item *);
    CREATE_FUNC_1(ItemSprite, const std::string &);

    static Item *makeItemByType(const std::string &itemtype);
    Item * getItem();
    void setItem(Item *item);

    void setItemTexture();
    void wasCollected();
    void wasUsed(class UnitSprite*);
protected:
    Item *_item;
};

#endif //ITEM_SPRITE_H__