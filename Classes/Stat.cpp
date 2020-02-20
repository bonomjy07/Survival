//
//  SurvivorStat.cpp
//  Survival
//
//  Created by jy_maeng on 2020/02/16.
//

#include "Stat.h"

Stat::Stat(float stamina, float thirsty, float hunger, float sleep)
: _maxStamina(stamina), _maxThirsty(thirsty), _maxHunger(hunger), _maxSleep(sleep)
{
    _currentStamina = _maxStamina;
    _currentThirsty = _maxThirsty;
    _currentHunger = _maxHunger;
    _currentSleep = _maxSleep;
}

Stat::~Stat() {}

float Stat::getMaxStamina() const
{
    return _maxStamina;
}

float Stat::getMaxThirsty() const
{
    return _maxThirsty;
}
float Stat::getMaxHunger() const
{
    return _maxHunger;
}
float Stat::getMaxSleep() const
{
    return _maxSleep;
}

float Stat::getCurrentStamina() const
{
    return _currentStamina;
}

float Stat::getCurrentThirsty() const
{
    return _currentThirsty;
}
float Stat::getCurrentHunger() const
{
    return _currentHunger;
}
float Stat::getCurrentSleep() const
{
    return _currentSleep;
}

void Stat::updateCurrentStamina(float deltaStamina)
{
    _currentStamina += deltaStamina;
}

void Stat::updateCurrentThirsty(float deltaThirsty)
{
    _currentThirsty += deltaThirsty;
}

void Stat::updateCurrentHunger(float deltaHunger)
{
    _currentHunger += deltaHunger;
}

void Stat::updateCurrentSleep(float deltaSleep)
{
    _currentSleep += deltaSleep;
}
