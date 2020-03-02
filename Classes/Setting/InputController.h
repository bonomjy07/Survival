//
//  InputController.h
//  Survival
//
//  Created by jy_maeng on 2020/03/01.
//

#pragma once

#ifndef InputController_h
#define InputController_h
class InputController
{
public:
    enum class InputEvent { KeyPressed = 0, KeyReleased };
    
private:
    struct MyPair
    {
        std::string action;
        InputEvent inputEvent;
        bool operator<(const MyPair& p) const // User-defined type key for map needs overloading operator<
        {
            if (this->action == p.action)
                return this->inputEvent < p.inputEvent;
            return this->action < p.action;
        }
    };
    std::map<MyPair, std::function<void(void*)>> inputBinder;

public:
    /* Binds action && input-event to function */
    void bindAction(const std::string& action, InputEvent inputEvent, std::function<void(void*)> func);
    
    /* Handles action */
    void takeAction(const std::string& action, InputEvent inputEvent);
};
#endif /* InputController_h */
