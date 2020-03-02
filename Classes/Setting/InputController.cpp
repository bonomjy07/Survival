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

#include <iostream>
void InputController::takeAction(const std::string &action, InputEvent inputEvent)
{
    auto it = inputBinder.find({action, inputEvent});
    if (it != inputBinder.end())
    {
        it->second(nullptr);
    }
}
