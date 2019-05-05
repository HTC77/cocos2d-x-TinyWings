#include "Hero.h"

// on "init" you need to initialize your instance
bool Hero::init()
{
    //////////////////////////////
    // 1. super init first
    if ( Sprite::init() && this->initWithFile("ball2.png"))
    {
		_awake = false;
	    visibleSize = Director::getInstance()->getVisibleSize();
	    winSize = Director::getInstance()->getWinSize();
	    origin = Director::getInstance()->getVisibleOrigin();
		this->createBody();
		this->wake();
		return true;
    }

    return false;
}

void Hero::wake()
{
	_awake = true;
	_body->setEnabled(true);
	_body->applyImpulse(Vec2(1, 2), _body->getPosition());
}

void Hero::dive()
{
	_body->applyImpulse(Vec2(-350, -250));
	this->limitDiveVelocity();
}

void Hero::limitVelocity()
{
	if (!_awake) return;
	const static float minVelocityX = 230;
	const static float minVelocityY = -77;
	Vec2 vel = _body->getVelocity();

	if (vel.x < minVelocityX)
		vel.x = minVelocityX;
	if (vel.y < minVelocityY)
		vel.y = minVelocityY;
	_body->setVelocity(vel);
}

void Hero::createBody()
{
	float radius = this->getContentSize().width/2;
	Vec2 startPosition = Vec2(0, winSize.height / 2 + radius);
	_body = PhysicsBody::createCircle(radius,
		PhysicsMaterial(1.0f, 0.0f, 0.2f));
	_body->setDynamic(true);
	_body->setLinearDamping(0.1f);
	this->setPhysicsBody(_body);
	this->setPosition(startPosition);
}

void Hero::limitDiveVelocity()
{
	if (!_awake) return;
	const static float minVelocityX = 120;
	const static float minVelocityY = -77;
	Vec2 vel = _body->getVelocity();

	if (vel.x < minVelocityX)
		vel.x = minVelocityX;
	if (vel.y < minVelocityY)
		vel.y = minVelocityY;
	_body->setVelocity(vel);
}

