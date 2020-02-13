
#include "KeyBinder.h"

std::map<cocos2d::EventKeyboard::KeyCode, std::string> KeyBinder::gameKeyTable;
KeyBinder::~KeyBinder(){}

bool KeyBinder::init()
{
    return true;
}

bool KeyBinder::setGameKey(cocos2d::EventKeyboard::KeyCode key, std::string value)
{
    gameKeyTable[key] = value;
}

std::string KeyBinder::findGameKey(cocos2d::EventKeyboard::KeyCode key)
{
    
    return gameKeyTable.find(key)->second;
}

void KeyBinder::loadGameKeys()
{
    gameKeyTable[cocos2d::EventKeyboard::KeyCode::KEY_W] = "Up";
    gameKeyTable[cocos2d::EventKeyboard::KeyCode::KEY_D] = "Right";
    gameKeyTable[cocos2d::EventKeyboard::KeyCode::KEY_A] = "Left";
    gameKeyTable[cocos2d::EventKeyboard::KeyCode::KEY_S] = "Down";
}