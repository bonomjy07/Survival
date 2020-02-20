#ifndef KEY_BINDER_H__
#define KEY_BINDER_H__

#include <string>
#include <map>

#include "cocos2d.h"

class KeyBinder : public cocos2d::Ref
{
public:
    KeyBinder();
    virtual ~KeyBinder();
    virtual bool init();

    bool checkGameKeyAction(cocos2d::EventKeyboard::KeyCode key, std::string action);
    
    void getNewGameKeyTable();
    std::string findNewGameKeyAction(cocos2d::EventKeyboard::KeyCode key);
    void setNewGameKeyAction(cocos2d::EventKeyboard::KeyCode key, std::string action);
    bool applyNewGameKey();

    static void initGameKeyActions();
    static std::string getKeyName(cocos2d::EventKeyboard::KeyCode key);
private:
    std::map<cocos2d::EventKeyboard::KeyCode, std::string>* getTable(int table);
    void setKeyAction(int table,cocos2d::EventKeyboard::KeyCode key, std::string action);
    std::string findAction(int table, cocos2d::EventKeyboard::KeyCode key);

private:
    std::map<cocos2d::EventKeyboard::KeyCode, std::string> newGameKeyTable;

    static std::map<cocos2d::EventKeyboard::KeyCode, std::string> gameKeyTable;

friend class KeyTableScene;
};

#endif // KEY_BIND_H__