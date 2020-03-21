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
    
    auto label = ui::Text::create("No Item", "res/font/arial.ttf", 12);
    if (label == nullptr)
    {
        log("Failed to create label in hand layer");
        return false;
    }

    //Create image view
    _image = ui::ImageView::create("res/pokemon/hand/hand32.png");
    if (_image == nullptr)
    {
        log("Failed to create image view in hand layer");
        return false;
    }
    
    auto layout = ui::Layout::create();
    if (layout == nullptr)
    {
        log("Failed to create layout");
        return false;
    }
    layout->setLayoutType(ui::Layout::Type::VERTICAL);
    //layout->setBackGroundColor(Color3B::GRAY);
    //layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    layout->setContentSize(Size(40, 50));
    layout->addChild(label);
    layout->addChild(_image);

    // Set where this layer to be placed
    this->setPosition({200, 30});
    this->addChild(layout);

    return true;
}

void HandLayer::setImageViewFile(const std::string& filename)
{
    if (_image)
    {
        _image->loadTexture(filename);
    }
}

