#ifndef KEY_BINDER_H__
#define KEY_BINDER_H__

#include <string>

#include "cocos2d.h"

class KeyBinder : public cocos2d::Ref
{
public:
    virtual bool init();

    CREATE_FUNC(KeyBinder);

    bool setKey(cocos2d::EventKeyboard::KeyCode key, std::string value);
    std::string find(cocos2d::EventKeyboard::KeyCode key);
protected:
    cocos2d::Map<cocos2d::EventKeyboard::KeyCode, std::string>keyTable;
};

#endif // KEY_BIND_H__