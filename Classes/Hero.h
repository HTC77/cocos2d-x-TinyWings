#ifndef __TinyWings__Hero__
#define __TinyWings__Hero__

#include "cocos2d.h"
USING_NS_CC;

class Hero : public Sprite
{
public:
    virtual bool init();
    
    Size visibleSize;
    Size winSize;
    Vec2 origin;
	bool _awake;
    // implement the "static create()" method manually
    CREATE_FUNC(Hero);
	void createBody();
};

#endif // __TinyWings__Hero__
