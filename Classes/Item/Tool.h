#ifndef TOOL_H__
#define TOOL_H__

#include "Item.h"

class Tool : public Item
{
public:
    Tool(const char * _des,  const char  * _img,
     const char * _thumb, const int _weight);
     ~Tool();

     virtual bool use()=0;
     virtual bool isMaterial()=0;
protected:
    int damage;
};

class Sword : public Tool
{
public:
    Sword();
    ~Sword();

    virtual bool use() override;
    virtual bool isMaterial() override;

private:
    static const char * const SWORD_DESCRIPTION;
    static const char * const SWORD_IMAGE_FILE_NAME;
    static const char * const SWORD_THUMBNAIL_FILE_NAME;
    static const int SWORD_WEIGHT;
};

#endif  // TOO_H__