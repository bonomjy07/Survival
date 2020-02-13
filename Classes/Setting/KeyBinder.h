#ifndef KEY_BINDER_H__
#define KEY_BINDER_H__

#include <string>

#include "cocos2d.h"

class KeyBinder : public cocos2d::Ref
{
public:
    virtual ~KeyBinder();
    virtual bool init();
    CREATE_FUNC(KeyBinder);

    void setGameKeyAction(cocos2d::EventKeyboard::KeyCode key, std::string value);
    std::string findGameKeyAction(cocos2d::EventKeyboard::KeyCode key);
    bool checkGameKeyAction(cocos2d::EventKeyboard::KeyCode key, std::string value);

public:
    static void loadGameKeyActions();
    static std::string getKeyName(cocos2d::EventKeyboard::KeyCode key);
public:
    static std::map<cocos2d::EventKeyboard::KeyCode, std::string> gameKeyTable;
};

#endif // KEY_BIND_H__