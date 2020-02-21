#include "GridView.h"

USING_NS_CC;

GridView::~GridView() {}

bool GridView::init()
{
    if (!ui::Layout::init())
        return false;

    setLayoutType(ui::Layout::Type::ABSOLUTE);
    rowCount = 2;
    columnCount = 2;
    maxCount = rowCount * columnCount;
    padding = 16;
    childMargin = 16;
    childMaxSize = Size(50, 50);

    setSize(Size(100, 100));
    
    // ui::Button *btn1 = ui::Button::create();
    // btn1->setTitleText("button1");
    // btn1->setColor(Color3B(80,0,0));
    // addChild(btn1);

    // ui::Button *btn2 = ui::Button::create();
    // btn2->setTitleText("button2");
    // btn2->setColor(Color3B(80,0,0));
    // addChild(btn2);

    // ui::Button *btn3 = ui::Button::create();
    // btn3->setTitleText("button3");
    // btn3->setColor(Color3B(80,0,0));
    // addChild(btn3);

    // ui::Button *btn4 = ui::Button::create();
    // btn4->setTitleText("button4");
    // btn4->setColor(Color3B(80,0,0));
    // addChild(btn4);
    
    // arrange();

    return true;
}

void GridView::setGrid(int row, int column)
{
    this->rowCount = row;
    this->columnCount = column;
    this->maxCount = rowCount * columnCount;
    float x = column * childMaxSize.width + (column-1) * childMargin + 2 * (padding);
    float y = row * childMaxSize.height + (row-1) * childMargin + 2 * (padding);
    Size gridSize(x, y);
    setSize(gridSize);
}

void GridView::arrange()
{
    Vec2 newPosition;
    Vec2 rowPosition;
    auto parentSize = getContentSize();
    float x = padding;
    float y = parentSize.height - (padding + childMaxSize.height);
    rowPosition = Vec2(x, y);
    newPosition = rowPosition;

    int rcount = 0;
    int ccount = 0;
    for ( auto child : _children ) {
        child->setPosition(newPosition);
        child->setContentSize(childMaxSize);    
        ccount++;
        if ( columnCount == ccount ) {
            float diff = childMaxSize.height + childMargin;
            newPosition = rowPosition + Vec2(0, -diff);
            rowPosition = newPosition;
            ccount = 0;
            rcount++;
        }
        else {
            float diff = childMaxSize.width + childMargin;
            newPosition = newPosition + Vec2(diff, 0);
        }

        if ( rcount == rowCount && ccount == columnCount ){
            break;
        }
    }
}
