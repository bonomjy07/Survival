#ifndef FOOD_H__
#define FOOD_H__

#include "Item.h"

class Food : public Item
{
public:
    Food( char const * _des,  char const * _img,
     char const * _thumb, int _weight);
    ~Food();

    virtual bool use()=0;
    virtual bool isMaterial()=0;
protected:
    // hunger recovery amount
    int hra;
    // thirst recovery amount
    int tra;
};

class DeerMeat : public Food
{
public:
    DeerMeat();
    DeerMeat(char const * _des, char const * _img,
     const char * _thumb, int _weight);
    ~DeerMeat();

    virtual bool use() override;
    virtual bool isMaterial() override;

private:
    static const char * const DEER_MEAT_DESCRIPTION;
    static const char * const DEER_MEAT_IMAGE_FILE_NAME;
    static const char * const DEER_MEAT_THUMBNAIL_FILE_NAME;
    static const int DEER_MEAT_WEIGHT;
};

#endif // FOOD_H__
