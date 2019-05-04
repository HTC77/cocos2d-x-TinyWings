#include "Hero.h"

// on "init" you need to initialize your instance
bool Hero::init()
{
    //////////////////////////////
    // 1. super init first
    if ( Sprite::init() && this->initWithSpriteFrameName("seal1.png"))
    {
		_awake = false;
	    visibleSize = Director::getInstance()->getVisibleSize();
	    winSize = Director::getInstance()->getWinSize();
	    origin = Director::getInstance()->getVisibleOrigin();
		this->createBody();
		return true;
    }

    return false;
}

void Hero::createBody()
{
	float radius = 16.0f;
	Vec2 startPosition = Vec2(0, winSize.height / 2 + radius);
	auto body = PhysicsBody::createCircle(radius,
		PhysicsMaterial(1.0f, 0.0f, 0.2f));
	body->setDynamic(true);
	body->setLinearDamping(0.1f);
	body->setRotationEnable(false);
	this->setPhysicsBody(body);
	this->setPosition(startPosition);
}

