#ifndef TOOL_H__
#define TOOL_H__

#include "Item.h"

class Tool : public Item
{
public:
    Tool(const char *name, const char *des, const char *img,
     const char *thumb, const int weight);
     ~Tool();

     virtual bool use()=0;
     virtual bool isMaterial()=0;

    float getDamage(){ return _damage; } 
protected:
    float _damage;
};

class Sword : public Tool
{
public:
    Sword();
    virtual ~Sword();
    virtual bool init();
    CREATE_FUNC(Sword);

    virtual bool use() override;
    virtual bool isMaterial() override;

private:
    static const char * const SWORD_NAME;
    static const char * const SWORD_DESCRIPTION;
    static const char * const SWORD_IMAGE_FILE_NAME;
    static const char * const SWORD_THUMBNAIL_FILE_NAME;
    static const int SWORD_WEIGHT;
};

#endif  // TOO_H__