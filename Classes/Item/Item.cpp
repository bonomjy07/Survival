
#include "Item.h"

Item::Item(const char * _des, const char * _img,
     const char * _thumb, const int _weight)
    :description(_des), imageFileName(_img),
     thumbnailFileName(_thumb), weight(_weight)
{

}

Item::~Item()
{

}

const char * Item::getDescription()
{
    return this->description;
}

const char * Item::getImageFileName()
{
    return this->imageFileName;
}

const char * Item::getThumbnailFileName()
{
    return this->thumbnailFileName;
}