//
//  HandLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/03/21.
//

#include "HandLayer.h"

USING_NS_CC;

bool HandLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Create image view
    _image = ui::ImageView::create();
    
    return true;
}

void HandLayer::setImageViewFile(const std::string& filename)
{
    if (_image)
    {
        _image->loadTexture(filename);
    }
}

