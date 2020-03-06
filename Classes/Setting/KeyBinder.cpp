
#include "KeyBinder.h"

USING_NS_CC;

std::map<EventKeyboard::KeyCode, std::string> KeyBinder::gameKeyTable;

std::string KeyBinder::NONE = "None";

KeyBinder::KeyBinder(){
    init();
}
KeyBinder::~KeyBinder(){
    log("keybinder deleted");
}

bool KeyBinder::init()
{
    getNewGameKeyTable();
    return true;
}

std::map<cocos2d::EventKeyboard::KeyCode, std::string>* KeyBinder::getTable(int table)
{
    std::map<cocos2d::EventKeyboard::KeyCode, std::string> *keyTable;
    switch(table){
        case 0:
            keyTable = &(this->gameKeyTable);
            break;
        case 1:
            keyTable = &(this->newGameKeyTable);
            break;
    }
    return keyTable;
}

void KeyBinder::setKeyAction(int table, EventKeyboard::KeyCode key, std::string action)
{
    std::map<cocos2d::EventKeyboard::KeyCode, std::string> *keyTable;
    keyTable = getTable(table);
    if( !keyTable )
        return;
    
    bool check = false;
    EventKeyboard::KeyCode tempKey;
    for(auto pair : *keyTable){
        if(!pair.second.compare(action)){
            tempKey = pair.first;
            check = true;
            break;
        }
    }
    if( check )
        (*keyTable).erase(tempKey);
    (*keyTable)[key] = action;
}

std::string KeyBinder::findAction(int table, EventKeyboard::KeyCode key)
{
    std::map<cocos2d::EventKeyboard::KeyCode, std::string> *keyTable;
    keyTable = getTable(table);
    if( !keyTable )
        return "None";
    if( (*keyTable).find(key) == (*keyTable).end() )
        return "None";
    return (*keyTable).find(key)->second;
}


bool KeyBinder::checkGameKeyAction(EventKeyboard::KeyCode key, std::string action)
{
    if( !findAction(0, key).compare(action) )
        return true;
    else
        return false;  
}

std::string KeyBinder::findGameKeyAction(cocos2d::EventKeyboard::KeyCode key)
{
    return findAction(0, key);
}

void KeyBinder::getNewGameKeyTable()
{
    newGameKeyTable.clear();
    for( auto pair : gameKeyTable ){
        newGameKeyTable.insert(pair);
    }
}

std::string KeyBinder::findNewGameKeyAction(cocos2d::EventKeyboard::KeyCode key)
{
    return findAction(1, key);
}

void KeyBinder::setNewGameKeyAction(cocos2d::EventKeyboard::KeyCode key, std::string action)
{
    setKeyAction(1, key, action);
}

bool KeyBinder::applyNewGameKey()
{
    gameKeyTable.clear();
    for( auto pair : newGameKeyTable ){
        gameKeyTable.insert(pair);
    }
}


void KeyBinder::initGameKeyActions()
{
    gameKeyTable[EventKeyboard::KeyCode::KEY_W] = "Up";
    gameKeyTable[EventKeyboard::KeyCode::KEY_D] = "Right";
    gameKeyTable[EventKeyboard::KeyCode::KEY_A] = "Left";
    gameKeyTable[EventKeyboard::KeyCode::KEY_S] = "Down";
    gameKeyTable[EventKeyboard::KeyCode::KEY_Z] = "Collect";
    gameKeyTable[EventKeyboard::KeyCode::KEY_SPACE] = "Use";
    gameKeyTable[EventKeyboard::KeyCode::KEY_F] = "Interact";
    gameKeyTable[EventKeyboard::KeyCode::KEY_I] = "Inventory";
}

std::string KeyBinder::getKeyName(EventKeyboard::KeyCode key)
{
    std::string ret = "KEY_UNKNOWN";
    switch(key){
        case EventKeyboard::KeyCode::KEY_NONE:
            ret = "KEY_NONE";
            break;
        case EventKeyboard::KeyCode::KEY_PAUSE:
            ret = "KEY_PAUSE";
            break;
        case EventKeyboard::KeyCode::KEY_SCROLL_LOCK:
            ret = "KEY_SCROLL_LOCK";
            break;
        case EventKeyboard::KeyCode::KEY_PRINT:
            ret = "KEY_PRINT";
            break;
        case EventKeyboard::KeyCode::KEY_SYSREQ:
            ret = "KEY_SYSREQ";
            break;
        case EventKeyboard::KeyCode::KEY_BREAK:
            ret = "KEY_BREAK";
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            ret = "KEY_ESCAPE";
            break;
        case EventKeyboard::KeyCode::KEY_BACKSPACE:
            ret = "KEY_BACKSPACE";
            break;
        case EventKeyboard::KeyCode::KEY_TAB:
            ret = "KEY_TAB";
            break;
        case EventKeyboard::KeyCode::KEY_BACK_TAB:
            ret = "KEY_BACK_TAB";
            break;
        case EventKeyboard::KeyCode::KEY_RETURN:
            ret = "KEY_RETURN";
            break;
        case EventKeyboard::KeyCode::KEY_CAPS_LOCK:
            ret = "KEY_CAPS_LOCK";
            break;
        case EventKeyboard::KeyCode::KEY_SHIFT:
            ret = "KEY_SHIFT";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_SHIFT:
            ret = "KEY_RIGHT_SHIFT";
            break;
        case EventKeyboard::KeyCode::KEY_CTRL:
            ret = "KEY_CTRL";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
            ret = "KEY_RIGHT_CTRL";
            break;
        case EventKeyboard::KeyCode::KEY_ALT:
            ret = "KEY_ALT";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ALT:
            ret = "KEY_RIGHT_ALT";
            break;
        case EventKeyboard::KeyCode::KEY_MENU:
            ret = "KEY_MENU";
            break;
        case EventKeyboard::KeyCode::KEY_HYPER:
            ret = "KEY_HYPER";
            break;
        case EventKeyboard::KeyCode::KEY_INSERT:
            ret = "KEY_INSERT";
            break;
        case EventKeyboard::KeyCode::KEY_HOME:
            ret = "KEY_HOME";
            break;
        case EventKeyboard::KeyCode::KEY_PG_UP:
            ret = "KEY_PG_UP";
            break;
        case EventKeyboard::KeyCode::KEY_DELETE:
            ret = "KEY_DELETE";
            break;
        case EventKeyboard::KeyCode::KEY_END:
            ret = "KEY_END";
            break;
        case EventKeyboard::KeyCode::KEY_PG_DOWN:
            ret = "KEY_PG_DOWN";
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            ret = "KEY_LEFT_ARROW";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            ret = "KEY_RIGHT_ARROW";
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            ret = "KEY_UP_ARROW";
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            ret = "KEY_DOWN_ARROW";
            break;
        case EventKeyboard::KeyCode::KEY_NUM_LOCK:
            ret = "KEY_NUM_LOCK";
            break;
        case EventKeyboard::KeyCode::KEY_KP_PLUS:
            ret = "KEY_KP_PLUS";
            break;
        case EventKeyboard::KeyCode::KEY_KP_MINUS:
            ret = "KEY_KP_MINUS";
            break;
        case EventKeyboard::KeyCode::KEY_KP_MULTIPLY:
            ret = "KEY_KP_MULTIPLY";
            break;
        case EventKeyboard::KeyCode::KEY_KP_DIVIDE:
            ret = "KEY_KP_DIVIDE";
            break;
        case EventKeyboard::KeyCode::KEY_KP_ENTER:
            ret = "KEY_KP_ENTER";
            break;
        case EventKeyboard::KeyCode::KEY_KP_HOME:
            ret = "KEY_KP_HOME";
            break;
        case EventKeyboard::KeyCode::KEY_KP_UP:
            ret = "KEY_KP_UP";
            break;
        case EventKeyboard::KeyCode::KEY_KP_PG_UP:
            ret = "KEY_KP_PG_UP";
            break;
        case EventKeyboard::KeyCode::KEY_KP_LEFT:
            ret = "KEY_KP_LEFT";
            break;
        case EventKeyboard::KeyCode::KEY_KP_FIVE:
            ret = "KEY_KP_FIVE";
            break;
        case EventKeyboard::KeyCode::KEY_KP_RIGHT:
            ret = "KEY_KP_RIGHT";
            break;
        case EventKeyboard::KeyCode::KEY_KP_END:
            ret = "KEY_KP_END";
            break;
        case EventKeyboard::KeyCode::KEY_KP_DOWN:
            ret = "KEY_KP_DOWN";
            break;
        case EventKeyboard::KeyCode::KEY_KP_PG_DOWN:
            ret = "KEY_KP_PG_DOWN";
            break;
        case EventKeyboard::KeyCode::KEY_KP_INSERT:
            ret = "KEY_KP_INSERT";
            break;
        case EventKeyboard::KeyCode::KEY_KP_DELETE:
            ret = "KEY_KP_DELETE";
            break;
        case EventKeyboard::KeyCode::KEY_F1:
            ret = "KEY_F1";
            break;
        case EventKeyboard::KeyCode::KEY_F2:
            ret = "KEY_F2";
            break;
        case EventKeyboard::KeyCode::KEY_F3:
            ret = "KEY_F3";
            break;
        case EventKeyboard::KeyCode::KEY_F4:
            ret = "KEY_F4";
            break;
        case EventKeyboard::KeyCode::KEY_F5:
            ret = "KEY_F5";
            break;
        case EventKeyboard::KeyCode::KEY_F6:
            ret = "KEY_F6";
            break;
        case EventKeyboard::KeyCode::KEY_F7:
            ret = "KEY_F7";
            break;
        case EventKeyboard::KeyCode::KEY_F8:
            ret = "KEY_F8";
            break;
        case EventKeyboard::KeyCode::KEY_F9:
            ret = "KEY_F9";
            break;
        case EventKeyboard::KeyCode::KEY_F10:
            ret = "KEY_F10";
            break;
        case EventKeyboard::KeyCode::KEY_F11:
            ret = "KEY_F11";
            break;
        case EventKeyboard::KeyCode::KEY_F12:
            ret = "KEY_F12";
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            ret = "KEY_SPACE";
            break;
        case EventKeyboard::KeyCode::KEY_EXCLAM:
            ret = "KEY_EXCLAM";
            break;
        case EventKeyboard::KeyCode::KEY_QUOTE:
            ret = "KEY_QUOTE";
            break;
        case EventKeyboard::KeyCode::KEY_NUMBER:
            ret = "KEY_NUMBER";
            break;
        case EventKeyboard::KeyCode::KEY_DOLLAR:
            ret = "KEY_DOLLAR";
            break;
        case EventKeyboard::KeyCode::KEY_PERCENT:
            ret = "KEY_PERCENT";
            break;
        case EventKeyboard::KeyCode::KEY_CIRCUMFLEX:
            ret = "KEY_CIRCUMFLEX";
            break;
        case EventKeyboard::KeyCode::KEY_AMPERSAND:
            ret = "KEY_AMPERSAND";
            break;
        case EventKeyboard::KeyCode::KEY_APOSTROPHE:
            ret = "KEY_APOSTROPHE";
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_PARENTHESIS:
            ret = "KEY_LEFT_PARENTHESIS";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_PARENTHESIS:
            ret = "KEY_RIGHT_PARENTHESIS";
            break;
        case EventKeyboard::KeyCode::KEY_ASTERISK:
            ret = "KEY_ASTERISK";
            break;
        case EventKeyboard::KeyCode::KEY_PLUS:
            ret = "KEY_PLUS";
            break;
        case EventKeyboard::KeyCode::KEY_COMMA:
            ret = "KEY_COMMA";
            break;
        case EventKeyboard::KeyCode::KEY_MINUS:
            ret = "KEY_MINUS";
            break;
        case EventKeyboard::KeyCode::KEY_PERIOD:
            ret = "KEY_PERIOD";
            break;
        case EventKeyboard::KeyCode::KEY_SLASH:
            ret = "KEY_SLASH";
            break;
        case EventKeyboard::KeyCode::KEY_0:
            ret = "KEY_0";
            break;
        case EventKeyboard::KeyCode::KEY_1:
            ret = "KEY_1";
            break;
        case EventKeyboard::KeyCode::KEY_2:
            ret = "KEY_2";
            break;
        case EventKeyboard::KeyCode::KEY_3:
            ret = "KEY_3";
            break;
        case EventKeyboard::KeyCode::KEY_4:
            ret = "KEY_4";
            break;
        case EventKeyboard::KeyCode::KEY_5:
            ret = "KEY_5";
            break;
        case EventKeyboard::KeyCode::KEY_6:
            ret = "KEY_6";
            break;
        case EventKeyboard::KeyCode::KEY_7:
            ret = "KEY_7";
            break;
        case EventKeyboard::KeyCode::KEY_8:
            ret = "KEY_8";
            break;
        case EventKeyboard::KeyCode::KEY_9:
            ret = "KEY_9";
            break;
        case EventKeyboard::KeyCode::KEY_COLON:
            ret = "KEY_COLON";
            break;
        case EventKeyboard::KeyCode::KEY_SEMICOLON:
            ret = "KEY_SEMICOLON";
            break;
        case EventKeyboard::KeyCode::KEY_LESS_THAN:
            ret = "KEY_LESS_THAN";
            break;
        case EventKeyboard::KeyCode::KEY_EQUAL:
            ret = "KEY_EQUAL";
            break;
        case EventKeyboard::KeyCode::KEY_GREATER_THAN:
            ret = "KEY_GREATER_THAN";
            break;
        case EventKeyboard::KeyCode::KEY_QUESTION:
            ret = "KEY_QUESTION";
            break;
        case EventKeyboard::KeyCode::KEY_AT:
            ret = "KEY_AT";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_A:
            ret = "KEY_CAPITAL_A";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_B:
            ret = "KEY_CAPITAL_B";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_C:
            ret = "KEY_CAPITAL_C";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_D:
            ret = "KEY_CAPITAL_D";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_E:
            ret = "KEY_CAPITAL_E";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_F:
            ret = "KEY_CAPITAL_F";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_G:
            ret = "KEY_CAPITAL_G";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_H:
            ret = "KEY_CAPITAL_H";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_I:
            ret = "KEY_CAPITAL_I";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_J:
            ret = "KEY_CAPITAL_J";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_K:
            ret = "KEY_CAPITAL_K";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_L:
            ret = "KEY_CAPITAL_L";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_M:
            ret = "KEY_CAPITAL_M";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_N:
            ret = "KEY_CAPITAL_N";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_O:
            ret = "KEY_CAPITAL_O";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_P:
            ret = "KEY_CAPITAL_P";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_Q:
            ret = "KEY_CAPITAL_Q";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_R:
            ret = "KEY_CAPITAL_R";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_S:
            ret = "KEY_CAPITAL_S";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_T:
            ret = "KEY_CAPITAL_T";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_U:
            ret = "KEY_CAPITAL_U";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_V:
            ret = "KEY_CAPITAL_V";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_W:
            ret = "KEY_CAPITAL_W";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_X:
            ret = "KEY_CAPITAL_X";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_Y:
            ret = "KEY_CAPITAL_Y";
            break;
        case EventKeyboard::KeyCode::KEY_CAPITAL_Z:
            ret = "KEY_CAPITAL_Z";
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_BRACKET:
            ret = "KEY_LEFT_BRACKET";
            break;
        case EventKeyboard::KeyCode::KEY_BACK_SLASH:
            ret = "KEY_BACK_SLASH";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
            ret = "KEY_RIGHT_BRACKET";
            break;
        case EventKeyboard::KeyCode::KEY_UNDERSCORE:
            ret = "KEY_UNDERSCORE";
            break;
        case EventKeyboard::KeyCode::KEY_GRAVE:
            ret = "KEY_GRAVE";
            break;
        case EventKeyboard::KeyCode::KEY_A:
            ret = "KEY_A";
            break;
        case EventKeyboard::KeyCode::KEY_B:
            ret = "KEY_B";
            break;
        case EventKeyboard::KeyCode::KEY_C:
            ret = "KEY_C";
            break;
        case EventKeyboard::KeyCode::KEY_D:
            ret = "KEY_D";
            break;
        case EventKeyboard::KeyCode::KEY_E:
            ret = "KEY_E";
            break;
        case EventKeyboard::KeyCode::KEY_F:
            ret = "KEY_F";
            break;
        case EventKeyboard::KeyCode::KEY_G:
            ret = "KEY_G";
            break;
        case EventKeyboard::KeyCode::KEY_H:
            ret = "KEY_H";
            break;
        case EventKeyboard::KeyCode::KEY_I:
            ret = "KEY_I";
            break;
        case EventKeyboard::KeyCode::KEY_J:
            ret = "KEY_J";
            break;
        case EventKeyboard::KeyCode::KEY_K:
            ret = "KEY_K";
            break;
        case EventKeyboard::KeyCode::KEY_L:
            ret = "KEY_L";
            break;
        case EventKeyboard::KeyCode::KEY_M:
            ret = "KEY_M";
            break;
        case EventKeyboard::KeyCode::KEY_N:
            ret = "KEY_N";
            break;
        case EventKeyboard::KeyCode::KEY_O:
            ret = "KEY_O";
            break;
        case EventKeyboard::KeyCode::KEY_P:
            ret = "KEY_P";
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            ret = "KEY_Q";
            break;
        case EventKeyboard::KeyCode::KEY_R:
            ret = "KEY_R";
            break;
        case EventKeyboard::KeyCode::KEY_S:
            ret = "KEY_S";
            break;
        case EventKeyboard::KeyCode::KEY_T:
            ret = "KEY_T";
            break;
        case EventKeyboard::KeyCode::KEY_U:
            ret = "KEY_U";
            break;
        case EventKeyboard::KeyCode::KEY_V:
            ret = "KEY_V";
            break;
        case EventKeyboard::KeyCode::KEY_W:
            ret = "KEY_W";
            break;
        case EventKeyboard::KeyCode::KEY_X:
            ret = "KEY_X";
            break;
        case EventKeyboard::KeyCode::KEY_Y:
            ret = "KEY_Y";
            break;
        case EventKeyboard::KeyCode::KEY_Z:
            ret = "KEY_Z";
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_BRACE:
            ret = "KEY_LEFT_BRACE";
            break;
        case EventKeyboard::KeyCode::KEY_BAR:
            ret = "KEY_BAR";
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_BRACE:
            ret = "KEY_RIGHT_BRACE";
            break;
        case EventKeyboard::KeyCode::KEY_TILDE:
            ret = "KEY_TILDE";
            break;
        case EventKeyboard::KeyCode::KEY_EURO:
            ret = "KEY_EURO";
            break;
        case EventKeyboard::KeyCode::KEY_POUND:
            ret = "KEY_POUND";
            break;
        case EventKeyboard::KeyCode::KEY_YEN:
            ret = "KEY_YEN";
            break;
        case EventKeyboard::KeyCode::KEY_MIDDLE_DOT:
            ret = "KEY_MIDDLE_DOT";
            break;
        case EventKeyboard::KeyCode::KEY_SEARCH:
            ret = "KEY_SEARCH";
            break;
        case EventKeyboard::KeyCode::KEY_DPAD_LEFT:
            ret = "KEY_DPAD_LEFT";
            break;
        case EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
            ret = "KEY_DPAD_RIGHT";
            break;
        case EventKeyboard::KeyCode::KEY_DPAD_UP:
            ret = "KEY_DPAD_UP";
            break;
        case EventKeyboard::KeyCode::KEY_DPAD_DOWN:
            ret = "KEY_DPAD_DOWN";
            break;
        case EventKeyboard::KeyCode::KEY_DPAD_CENTER:
            ret = "KEY_DPAD_CENTER";
            break;
        case EventKeyboard::KeyCode::KEY_ENTER:
            ret = "KEY_ENTER";
            break;
        case EventKeyboard::KeyCode::KEY_PLAY:
            ret = "KEY_PLAY";
            break;
    }
    return ret;
}
