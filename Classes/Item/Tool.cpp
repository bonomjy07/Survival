#include "cocos2d.h"

#include "Tool.h"

USING_NS_CC;

Tool::Tool(const char *name, const char *des, const char *img,
    const char *thumb, const int weight)
    : Item(name, des, img, thumb, weight)
{

}
Tool::~Tool(){}

const char * const Sword::SWORD_NAME = "Sword";
const char * const Sword::SWORD_DESCRIPTION = "Sword";
const char * const Sword::SWORD_IMAGE_FILE_NAME = "res/TestResource/items/Sword.png";
const char * const Sword::SWORD_THUMBNAIL_FILE_NAME = "res/TestResource/items/Sword.png";
const int Sword::SWORD_WEIGHT = 100;

Sword::Sword()
    : Tool(SWORD_NAME, SWORD_DESCRIPTION, SWORD_IMAGE_FILE_NAME,
    SWORD_THUMBNAIL_FILE_NAME, SWORD_WEIGHT)
{}
Sword::~Sword(){}

bool Sword::init(){
    return true;
}

bool Sword::use(){
    return false;
}

bool Sword::isMaterial(){
    return false;
}