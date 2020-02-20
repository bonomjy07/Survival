//
//  StatLayer.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/17.
//

#include "StatLayer.h"
#include "Stat.h"

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

    std::string texts[4] = {"Stmina", "Thirsty", "Hunger", "Sleep"};
    for (int i = 0; i < 4; ++i)
    {
        if (auto list = ui::ListView::create())
        {
            if ((_texts[i] = ui::Text::create(texts[i], "fonts'arial.ttf", 16)))
            {
                _texts[i]->setContentSize(Size(60, 20));
                _texts[i]->ignoreContentAdaptWithSize(false);
                list->pushBackCustomItem(_texts[i]);
            }
            if ((_bars[i] = ui::LoadingBar::create("res/TestResource/TileImage/img_block.png")))
            {
                _bars[i]->setDirection(ui::LoadingBar::Direction::LEFT);
                _bars[i]->setPercent(100);
                list->pushBackCustomItem(_bars[i]);
            }
            if (list->getItems().size() == 2)
            {
                list->setContentSize(Size(_bars[i]->getContentSize().width + _texts[i]->getContentSize().width, _bars[i]->getContentSize().height));
                list->setDirection(ui::ListView::Direction::HORIZONTAL);
                statList->pushBackCustomItem(list);
            }
        }
    }
    
    this->addChild(statList);
    return true;
}

void StatLayer::update(float dt)
{
    _bars[static_cast<int>(Stat::StatName::Stamina)]->setPercent(_stat.getCurrentStamina()/                _stat.getMaxStamina()*100.f);
    _bars[static_cast<int>(Stat::StatName::Thirsty)]->setPercent(_stat.getCurrentThirsty()/                _stat.getMaxThirsty()*100.f);
    _bars[static_cast<int>(Stat::StatName::Hunger)]->setPercent(_stat.getCurrentHunger()/                _stat.getMaxHunger()*100.f);
    _bars[static_cast<int>(Stat::StatName::Sleep)]->setPercent(_stat.getCurrentSleep()/                _stat.getMaxSleep()*100.f);
}

StatLayer::StatLayer(const Stat& stat) : _stat(stat)
{
}

StatLayer::~StatLayer()
{
}
