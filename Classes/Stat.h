//
//  SurvivorStat.h
//  Survival
//
//  Created by jy_maeng on 2020/02/16.
//

#pragma once

#ifndef SurvivorStat_h
#define SurvivorStat_h

class Stat
{
public:
    Stat(float stamina = 100.f, float thirsty = 100.f, float hunger = 100.f, float sleep = 100.f);
    virtual ~Stat();
    
    enum class StatName {Stamina = 0, Thirsty, Hunger, Sleep};
    
public:
    float getMaxStamina() const;
    float getMaxThirsty() const;
    float getMaxHunger() const;
    float getMaxSleep() const;
    
    float getCurrentStamina() const;
    float getCurrentThirsty() const;
    float getCurrentHunger() const;
    float getCurrentSleep() const;
    
    void updateCurrentStamina(float deltaStamina);
    void updateCurrentThirsty(float deltaThirsty);
    void updateCurrentHunger(float deltaHunger);
    void updateCurrentSleep(float deltaSleep);
    
private:
    float _maxStamina;
    float _maxThirsty;
    float _maxHunger;
    float _maxSleep;
    
    float _currentStamina;
    float _currentThirsty;
    float _currentHunger;
    float _currentSleep;
};
#endif /* SurvivorStat_h */
