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
    enum class InputEvent { KeyPressed = 1, KeyReleased };
    
private:
    struct MyPair
    {
        std::string action;
        InputEvent inputEvent;
        
        bool operator==(const MyPair& other) const
        {
            return ((this->action == other.action) &&
                    (this->inputEvent == other.inputEvent));
        }
    };
    
    struct MyHashFunction
    {
        size_t operator()(const MyPair& p) const
        {
            size_t h1 = std::hash<std::string>{}(p.action);
            size_t h2 = std::hash<int>{}(static_cast<int>(p.inputEvent));
            return h1^(h2<<1);
        }
    };
    
    std::unordered_map<MyPair, std::function<void(void*)>, MyHashFunction> inputBinder;
    
public:
    /* Binds action && input-event to function */
    void bindAction(const std::string& action, InputEvent inputEvent, std::function<void(void*)> func);
    #define BIND_ACTION(__selector__) std::bind(&__selector__, this, std::placeholders::_1, std::placeholders::_2)
    
    /* Handles action */
    //void takeAction(const std::string& action, InputEvent inputEvent);
    void takeAction(std::string action, InputEvent inputevent);

public:
    /* before action */
    std::function<void (std::string, InputEvent)> onPreAction;
    /* if InputEvent is KeyPressed */
    std::function<void (std::string, InputEvent)> onPressed;
    /* if InputEvent is KeyReleased */
    std::function<void (std::string, InputEvent)> onReleased;
    /* after action */
    std::function<void (std::string, InputEvent)> onPostAction;
};
#endif /* InputController_h */
