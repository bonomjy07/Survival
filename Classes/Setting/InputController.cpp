//
//  InputController.cpp
//  Survival
//
//  Created by jy_maeng on 2020/03/01.
//

#include "InputController.h"

void InputController::bindAction(const std::string &action, InputController::InputEvent inputEvent, std::function<void (void *)> func)
{
    inputBinder.insert({{action, inputEvent}, func});
}

void InputController::bindAction(const std::string& action, std::function<void (void*, void*)> func){
    doActionBinder.insert({action, func});
}

void InputController::takeAction(const std::string &action, InputEvent inputEvent)
{
    auto it = inputBinder.find({action, inputEvent});
    if (it != inputBinder.end())
    {
        it->second(nullptr);
    }
}

void InputController::takeAction(const std::string &action, void *arg1, void *arg2)
{
    auto it = doActionBinder.find(action);
    if (it != doActionBinder.end())
    {
        it->second(arg1, arg2);
    }
}
