#ifndef KEY_BINDER_H__
#define KEY_BINDER_H__

#include <string>

#include "cocos2d.h"

class KeyBinder : public cocos2d::Ref
{
public:
    virtual ~KeyBinder();

    virtual bool init();

    bool setGameKey(cocos2d::EventKeyboard::KeyCode key, std::string value);
    std::string findGameKey(cocos2d::EventKeyboard::KeyCode key);
    void loadGameKeys();

    CREATE_FUNC(KeyBinder);
protected:
    static std::map<cocos2d::EventKeyboard::KeyCode, std::string> gameKeyTable;
};

#endif // KEY_BIND_H__