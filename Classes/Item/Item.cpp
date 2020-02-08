
#include "Item.h"

Item::Item(const char *_des, const char *_img,
     const char *_thumb, int _weight)
    :description(_des), imageFileName(_img),
     thumbnailFilename(_thumb), weight(_weight)
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
    return this->thumbnailFilename;
}