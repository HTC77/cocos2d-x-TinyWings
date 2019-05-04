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
	Scene* scene = Scene::createWithPhysics();
	HelloWorld* layer = HelloWorld::create();
	scene->addChild(layer);
	PhysicsWorld* world = scene->getPhysicsWorld();
	Vec2 gravity = Vec2(0, -98.0f);
	world->setGravity(gravity);
	world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	winSize = Director::getInstance()->getWinSize();
	origin = Director::getInstance()->getVisibleOrigin();

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
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...


	// background + terrain
	_background = new Sprite();
	_stripes = new Sprite();
	_terrain = ::Terrain::create();
	this->addChild(_terrain);
	this->genBackground();

	// touch listener
	auto touchListener =
		EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(
		touchListener, 27);

	this->setGLProgram(
		GLProgramCache::getInstance()->getGLProgram(
			GLProgram::SHADER_NAME_POSITION_COLOR));

	// hero
	_hero = Hero::create();
	_terrain->_batchNode->addChild(_hero);

	scheduleUpdate();
	
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
	_stripes->removeFromParentAndCleanup(true);

	Color4F bgColor = randomBrightColor();
	_background = spriteWithColor(512, 512, bgColor);
	Texture2D::TexParams p = { GL_LINEAR, GL_LINEAR,
		GL_REPEAT, GL_REPEAT };
	_background->getTexture()->setTexParameters(p);
	_background->setPosition(visibleSize / 2);
	this->addChild(_background, -1);

	Color4F color2 = randomBrightColor();
	int stripes = (rand() % 4 + 1) * 2;
	_stripes = stripedSpriteWithColor(512, 512, bgColor, color2, stripes);
	_stripes->setPosition(Vec2(visibleSize.width / 2, -visibleSize.height));
	_stripes->getTexture()->setAntiAliasTexParameters();
	_stripes->getTexture()->setTexParameters(p);
	this->_terrain->_stripes = _stripes;
	this->addChild(_stripes);
}

Sprite* HelloWorld::stripedSpriteWithColor(float textureWidth, float textureHeight, Color4F bgColor, Color4F color2,
	int nStripes)
{
	RenderTexture *rt = RenderTexture::create(textureWidth, textureHeight); // init method changed
	rt->setKeepMatrix(true);
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// noise
	Sprite* noise = Sprite::create("Noise.png");
	noise->setBlendFunc({ GL_DST_COLOR, GL_ZERO });
	noise->setPosition(Vec2(textureWidth / 2.0f, textureHeight / 2.0f));
	noise->visit();

	// Draw on texture
	_stripesCommand.init(rt->getGlobalZOrder());
	_stripesCommand.func = CC_CALLBACK_0(HelloWorld::onDrawStripes,
		this, textureWidth, textureHeight, nStripes, color2);
	auto renderer = Director::getInstance()->getRenderer();
	renderer->addCommand(&_stripesCommand);

	rt->end();
	return Sprite::createWithTexture(rt->getSprite()->getTexture());
}

Sprite* HelloWorld::spriteWithColor(float textureWidth, float textureHeight,
	Color4F bgColor)
{
	// 1: Create CCRenderTexture
	RenderTexture *rt = RenderTexture::create(textureWidth, textureHeight); // init method changed
	rt->setKeepMatrix(true);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// 3: Draw on texture


	//draw gradient
	_customCommand.init(rt->getGlobalZOrder());
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw,
		this, textureWidth, textureHeight);
	auto renderer = Director::getInstance()->getRenderer();
	renderer->addCommand(&_customCommand);

	//add noise
	Sprite* noise = Sprite::create("Noise.png");
	noise->setBlendFunc({ GL_DST_COLOR, GL_ZERO });
	noise->setPosition(Vec2(textureWidth/2.0f, textureHeight/2.0f));
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

	// ball
	auto circleBody = PhysicsBody::createCircle(25);
	circleBody->setDynamic(true);
	auto sprite = Sprite::create();
	sprite->setPosition(
		Director::getInstance()->convertToGL(
			touch->getLocationInView()));
	sprite->setPhysicsBody(circleBody);
	this->addChild(sprite);

	return true;
}

void HelloWorld::update(float delta)
{
	static float offsetX = 0;
	offsetX = _hero->getPositionX();
	Size textureSize = _background->getTextureRect().size;
	_background->setTextureRect(Rect(offsetX, 0,
		textureSize.width, textureSize.height));
	_terrain->setOffsetX(offsetX);
}

void HelloWorld::onDraw(float textureWidth, float textureHeight)
{
	CC_NODE_DRAW_SETUP();

	float gradientAlpha = 0.7f;
	int nVertices = 0;
	Color4F colors[4];
	Point vertices[4];

	vertices[nVertices] = Vec2(0, 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);
	vertices[nVertices] = Vec2(textureWidth, 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);
	vertices[nVertices] = Vec2(0, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);
	vertices[nVertices] = Vec2(textureWidth, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2,
		GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4,
		GL_FLOAT, GL_FALSE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(nVertices));

}

void HelloWorld::onDrawStripes(float textureWidth, float textureHeight, int nStripes, Color4F color2)
{
	CC_NODE_DRAW_SETUP();

	Color4F colors[48];
	Point vertices[48];
	int nVertices = 0;

	// stripes
	float x1 = -textureHeight;
	float x2 = x1 + textureHeight;
	float y1 = textureHeight;
	float y2 = 0;
	float dx = textureWidth / nStripes * 2;
	float stripeWidth = dx / 2;
	for (int i = 0; i < nStripes; ++i)
	{
		x2 = x1 + textureHeight;

		vertices[nVertices] = Point(x1, y1);
		colors[nVertices++] = Color4F(color2.r, color2.g, color2.b, color2.a);

		vertices[nVertices] = Point(x1 + stripeWidth, y1);
		colors[nVertices++] = Color4F(0, 0, 0, 1);

		vertices[nVertices] = Point(x2, y2);
		colors[nVertices++] = Color4F(color2.r, color2.g, color2.b, color2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = Color4F(color2.r, color2.g, color2.b, color2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = Color4F(color2.r, color2.g, color2.b, color2.a);

		vertices[nVertices] = Point(x2 + stripeWidth, y2);
		colors[nVertices++] = Color4F(color2.r, color2.g, color2.b, color2.a);

		x1 += dx;
	}
	CC_NODE_DRAW_SETUP();

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2,
		GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4,
		GL_FLOAT, GL_TRUE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(nVertices));

	// // gradient
	// float gradientAlpha = 0.7f;
	// nVertices = 0;
	// textureWidth = (int)(textureWidth * 3);
	// textureWidth = (int)(textureHeight * 3);
	// vertices[nVertices] = Vec2(0, 0);
	// colors[nVertices++] = Color4F(0, 0, 0, 0);
	// vertices[nVertices] = Vec2(textureWidth, 0);
	// colors[nVertices++] = Color4F(0, 0, 0, 0);
	// vertices[nVertices] = Vec2(0, textureHeight);
	// colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);
	// vertices[nVertices] = Vec2(textureWidth, textureHeight);
	// colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);
	// CC_NODE_DRAW_SETUP();
	//
	// GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_POSITION);
	// glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2,
	// 	GL_FLOAT, GL_FALSE, 0, vertices);
	// glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4,
	// 	GL_FLOAT, GL_FALSE, 0, colors);
	// glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	// glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(nVertices));
}

void HelloWorld::onEnter()
{
	Scene::onEnter();

	_world = Director::getInstance()->getRunningScene()->getPhysicsWorld();

}