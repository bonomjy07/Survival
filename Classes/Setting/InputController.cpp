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

// void InputController::takeAction(const std::string &action, InputEvent inputEvent)
// {
//     auto it = inputBinder.find({action, inputEvent});
//     if (it != inputBinder.end())
//     {
//         it->second(nullptr);
//     }
// }

void InputController::takeAction(std::string action, InputEvent inputevent)
{
    if ( onPreAction != nullptr )
        onPreAction(action, inputevent);
    if ( onPressed != nullptr && inputevent == InputEvent::KeyPressed )
        onPressed(action, inputevent);
    if ( onReleased != nullptr && inputevent == InputEvent::KeyReleased )
        onReleased(action, inputevent);
    if ( onPostAction != nullptr )
        onPostAction(action, inputevent);
}
