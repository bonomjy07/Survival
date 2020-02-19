//
//  MySprite.h
//  Survival
//
//  Created by jy_maeng on 2020/02/18.
//

#pragma once

#ifndef MySprite_h
#define MySprite_h

class MySprite : public cocos2d::Sprite
{
public:
    virtual ~MySprite();
    
protected:
public:
    /**
     @brief Creates physics body and adds it to sprite
     @details It has to be called when a sprite is created
     @warning If this fucntion is finished without error, it'll activate update(float dt) automatically.....
     */
    virtual bool initPhysicsBody();
};
#endif /* MySprite_h */
