//
//  StatLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/17.
//

#include "StatLayer.h"
#include "Stat.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

StatLayer* StatLayer::create(const Stat& stat)
{
    StatLayer* layer = new(std::nothrow) StatLayer(stat);
    if (layer && layer->init())
    {
        layer->setName("StatLayer");
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool StatLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    ui::ListView* statList;
    if (!(statList = ui::ListView::create()))
    {
        return false;
    }
    statList->setContentSize(Size(200, 200));
    statList->setGravity(ui::ListView::Gravity::LEFT);
    statList->setDirection(ui::ListView::Direction::VERTICAL);
    statList->setItemsMargin(0);

    if (auto list = ui::ListView::create())
    {
        auto text = ui::Text::create("stamina", "fonts/arial.ttf", 16);
        text->setContentSize(Size(60, 20));
        text->ignoreContentAdaptWithSize(false);
        auto bar = ui::LoadingBar::create("res/TestResource/TileImage/img_block.png");
        bar->setDirection(ui::LoadingBar::Direction::RIGHT);
        bar->setPercent(100);
        list->setContentSize(Size(bar->getContentSize().width + text->getContentSize().width, bar->getContentSize().height));
        list->setDirection(ui::ListView::Direction::HORIZONTAL);
        list->pushBackCustomItem(text);
        list->pushBackCustomItem(bar);
        statList->pushBackCustomItem(list);
    }
    if (auto list = ui::ListView::create())
    {
        auto text = ui::Text::create("Thirsty", "fonts/arial.ttf", 16);
        text->setContentSize(Size(60, 20));
        text->ignoreContentAdaptWithSize(false);
        auto bar = ui::LoadingBar::create("res/TestResource/TileImage/img_block.png");
        bar->setDirection(ui::LoadingBar::Direction::RIGHT);
        bar->setPercent(100);
        list->setContentSize(Size(bar->getContentSize().width + text->getContentSize().width, bar->getContentSize().height));
        list->setDirection(ui::ListView::Direction::HORIZONTAL);
        list->pushBackCustomItem(text);
        list->pushBackCustomItem(bar);
        statList->pushBackCustomItem(list);
    }
    if (auto list = ui::ListView::create())
    {
        auto text = ui::Text::create("Hunger", "fonts/arial.ttf", 16);
        text->setContentSize(Size(60, 20));
        text->ignoreContentAdaptWithSize(false);
        auto bar = ui::LoadingBar::create("res/TestResource/TileImage/img_block.png");
        bar->setDirection(ui::LoadingBar::Direction::RIGHT);
        bar->setPercent(100);
        list->setContentSize(Size(bar->getContentSize().width + text->getContentSize().width, bar->getContentSize().height));
        list->setDirection(ui::ListView::Direction::HORIZONTAL);
        list->pushBackCustomItem(text);
        list->pushBackCustomItem(bar);
        statList->pushBackCustomItem(list);
    }
    if (auto list = ui::ListView::create())
    {
        auto text = ui::Text::create("Sleep", "fonts/arial.ttf", 16);
        text->setContentSize(Size(60, 20));
        text->ignoreContentAdaptWithSize(false);
        auto bar = ui::LoadingBar::create("res/TestResource/TileImage/img_block.png");
        bar->setDirection(ui::LoadingBar::Direction::RIGHT);
        bar->setPercent(100);
        list->setContentSize(Size(bar->getContentSize().width + text->getContentSize().width, bar->getContentSize().height));
        list->setDirection(ui::ListView::Direction::HORIZONTAL);
        list->pushBackCustomItem(text);
        list->pushBackCustomItem(bar);
        statList->pushBackCustomItem(list);
    }
    
    this->addChild(statList);
    return true;
}

StatLayer::StatLayer(const Stat& stat) : _stat(stat)
{
}

StatLayer::~StatLayer()
{
}
