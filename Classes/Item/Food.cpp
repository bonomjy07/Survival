
#include "cocos2d.h"

#include "Food.h"

USING_NS_CC;

Food::Food(char const* _des,  char const* _img,
      char const* _thumb, int _weight)
    : Item(_des, _img, _thumb, _weight)
{

}

Food::~Food()
{

}

// Deer Meat class initialize
const char * const DeerMeat::DEER_MEAT_DESCRIPTION = "Deer Meat";
const char * const DeerMeat::DEER_MEAT_IMAGE_FILE_NAME = "res/TestResource/items/RawMeat.png";
const char * const DeerMeat::DEER_MEAT_THUMBNAIL_FILE_NAME = "res/TestResource/items/RawMeat.png";
const int DeerMeat::DEER_MEAT_WEIGHT = 10;

DeerMeat::DeerMeat()
    : Food(DEER_MEAT_DESCRIPTION, DEER_MEAT_IMAGE_FILE_NAME,
     DEER_MEAT_THUMBNAIL_FILE_NAME, DEER_MEAT_WEIGHT)
{

}
DeerMeat::~DeerMeat()
{
    log("DeerMeat deleted");
}

bool DeerMeat::use(){
    return false;
}

bool DeerMeat::isMaterial(){
    return false;
}