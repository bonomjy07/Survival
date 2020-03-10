#ifndef CREATE_H__
#define CREATE_H__

#include "cocos2d.h"

#define CREATE_FUNC_1(__TYPE__, __ARG_TYPE__) \
static __TYPE__* create(__ARG_TYPE__ arg) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(arg)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

#endif //CREATE_H__