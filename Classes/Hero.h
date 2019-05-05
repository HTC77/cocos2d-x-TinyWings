#ifndef __TinyWings__Hero__
#define __TinyWings__Hero__

#include "cocos2d.h"
USING_NS_CC;

class Hero : public Sprite
{
public:
    virtual bool init();
   	CREATE_FUNC(Hero);
 
    Size visibleSize;
    Size winSize;
    Vec2 origin;
	bool _awake;
	void wake();
	void dive();
	void limitVelocity();
	PhysicsBody* _body;
	void createBody();
private:
	void limitDiveVelocity();
};

#endif // __TinyWings__Hero__
