
#include "KeyBinder.h"

bool KeyBinder::init()
{
}

bool KeyBinder::setKey(cocos2d::EventKeyboard::KeyCode key, std::string value)
{
    this->keyTable.insert(key, value);
}

std::string KeyBinder::find(cocos2d::EventKeyboard::KeyCode key)
{
    return this->keyTable.at(key);
}