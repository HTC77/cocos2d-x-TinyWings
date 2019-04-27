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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    winSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	
	// background
	_background = new Sprite();
	this->genBackground();
	
	// touch listener
	auto touchListener =
		EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(
		touchListener, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::genBackground()
{
	_background->removeFromParentAndCleanup(true);
	Color4F bgColor = randomBrightColor();
	_background = spriteWithColor(512, 512, bgColor);
	_background->setPosition(visibleSize / 2);

	this->addChild(_background);
}

Sprite* HelloWorld::spriteWithColor(int textureWidth, int textureHeight,
	Color4F bgColor)
{
	// 1: Create CCRenderTexture
	RenderTexture *rt = RenderTexture::create(textureWidth, textureHeight);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	
	// 3: Draw on texture

	// draw gradiant
	float gradiantAlpha = 0.7;
	int nVertices = 0;
	Color4F colors[48];
	Point vertices[48];
	this->setGLProgram(
		GLProgramCache::getInstance()->getGLProgram(
			GLProgram::SHADER_NAME_POSITION_COLOR));

	CC_NODE_DRAW_SETUP();
	
	vertices[nVertices] = Point(0, 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	vertices[nVertices] = Point(textureWidth, 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	vertices[nVertices] = Point(0, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradiantAlpha);

	vertices[nVertices] = Point(textureWidth, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradiantAlpha);

	glEnable(GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_POSITION);
	glVertexAttribPointer(GL::VERTEX_ATTRIB_FLAG_POSITION, 2,
		GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GL::VERTEX_ATTRIB_FLAG_COLOR, 4,
		GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(nVertices));

	// add noise
	BlendFunc blendFunc;
	blendFunc.dst = GL_ZERO;
	blendFunc.src = GL_DST_COLOR;
	Sprite* noise = Sprite::create("Noise-hd.png");
	noise->setBlendFunc(blendFunc);
	noise->setPosition(Vec2(textureWidth / 2, textureHeight / 2));
	noise->visit();

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: create new sprite from the texture
	return Sprite::createWithTexture(rt->getSprite()->getTexture());
}

Color4F HelloWorld::randomBrightColor()
{
	int requiredBrightness = 192;
	Color4B color;
	while (true)
	{
		color = Color4B(rand() % 255, rand() % 255, rand() % 255, 255);
		if (color.r > requiredBrightness
			|| color.g > requiredBrightness
			|| color.b > requiredBrightness)
			break;
	}
	return Color4F(color);
}

bool HelloWorld::touchBegan(Touch* touch, Event* event)
{
	this->genBackground();
	return true;
}
