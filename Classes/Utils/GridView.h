#ifndef GRID_VIEW_H__
#define GRID_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GridView : public cocos2d::ui::Layout
{
public:
    virtual ~GridView();
    virtual bool init();
    CREATE_FUNC(GridView);

    void setPadding(float _padding){ padding = _padding; }
    float getPadding(){ return padding; }
    void setChildMargin(float margin){ childMargin = margin; }
    float getChildMargin(){ return childMargin; }
    
    int getMaxCount(){ return maxCount; }

    void setGrid(int row, int height);
    void arrange();

protected:
    int rowCount;
    int columnCount;
    int maxCount;

    float padding;
    float childMargin;

    cocos2d::Size childMaxSize;

};

#endif GRID_VIEW_H__
