/****************************************************************************
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Terrain.h"
#include "Hero.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
	Size visibleSize;
	Size winSize;
	Vec2 origin;
	void genBackground();
	Sprite* _background;
	Sprite* stripedSpriteWithColor(float textureWidth, float textureHeight,
		Color4F bgColor, Color4F color2, int nStripes);
	Sprite* spriteWithColor(float textureWidth, float textureHeight, Color4F bgColor);
	Color4F randomBrightColor();
	bool touchBegan(Touch* touch, Event* event);
	void update(float delta) override;
	CustomCommand _customCommand;
	CustomCommand _stripesCommand;
	void onDraw(float textureWidth, float textureHeight);
	void onDrawStripes(float textureWidth, float textureHeight,
		int nStripes, Color4F color2);
	::Terrain* _terrain;
	Sprite* _stripes;
	PhysicsWorld* _world;
	void onEnter() override;

	Hero* _hero;
	bool _tabDown;
	void touchEnded(Touch* touch, Event* event);
	void touchCancelled(Touch* touch, Event* event);

};

#endif // __HELLOWORLD_SCENE_H__
