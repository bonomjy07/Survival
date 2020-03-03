
#include "cocos2d.h"

#include "Food.h"

USING_NS_CC;

Food::Food(const char *name, const char *des, const char *img,
    const char *thumb, const int weight)
    : Item(name, des, img, thumb, weight)
{

}

Food::~Food()
{

}

// Deer Meat class initialize
const char * const DeerMeat::DEER_MEAT_NAME = "DeerMeat";
const char * const DeerMeat::DEER_MEAT_DESCRIPTION = "Deer Meat";
const char * const DeerMeat::DEER_MEAT_IMAGE_FILE_NAME = "res/TestResource/items/RawMeat.png";
const char * const DeerMeat::DEER_MEAT_THUMBNAIL_FILE_NAME = "res/TestResource/items/RawMeat.png";
const int DeerMeat::DEER_MEAT_WEIGHT = 10;

DeerMeat::DeerMeat()
    : Food(DEER_MEAT_NAME, DEER_MEAT_DESCRIPTION, DEER_MEAT_IMAGE_FILE_NAME,
     DEER_MEAT_THUMBNAIL_FILE_NAME, DEER_MEAT_WEIGHT)
{

}
DeerMeat::~DeerMeat()
{
    log("DeerMeat deleted");
}
bool DeerMeat::init(){
    return true;
}

bool DeerMeat::use(){
    return false;
}

bool DeerMeat::isMaterial(){
    return false;
}
