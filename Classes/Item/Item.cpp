
#include "Item.h"

Item::Item(const char *name, const char *des, const char *img,
    const char *thumb, const int weight)
    : _name(name), _description(des), _imageFileName(img),
    _thumbnailFileName(thumb), _weight(weight)
{

}

Item::~Item(){}