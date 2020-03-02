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
    template<class T1, class T2>
    struct MyPair
    {
        T1 comm;
        T2 ie;
        bool operator<(const MyPair& p) const
        {
            return this->comm < p.comm;
        }
    };
    std::map<MyPair<std::string, InputEvent>, std::function<void(void*)>> inputBinder;

public:
    void bindAction(const std::string& action, InputEvent inputEvent, std::function<void(void*)> func);
    void takeAction(const std::string& action, InputEvent inputEvent);
};

#endif /* InputController_h */
