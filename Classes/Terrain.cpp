#include "Terrain.h"

// on "init" you need to initialize your instance
bool ::Terrain::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    winSize = Director::getInstance()->getWinSize();

	_fromKeyPointI = 0;
	_toKeyPointI = 0;
	drawNode = DrawNode::create();
	this->addChild(drawNode);
	this->generateHills();
	this->resetHillVertices();

    return true;
}

void ::Terrain::resetHillVertices()
{
	// key point interval for drawing
	while (_hillKeyPoints[_fromKeyPointI + 1].x < -winSize.width / 8 / this->getScale())
	{
		_fromKeyPointI++;
	}
	while (_hillKeyPoints[_toKeyPointI].x < winSize.width * 12 / 8 / this->getScale())
	{
		_toKeyPointI++;
	}
}

void ::Terrain::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(::Terrain::onDrawHills, this);
	renderer->addCommand(&_customCommand);
}

void ::Terrain::onDrawHills()
{
	drawNode->clear();
	for (int i = MAX(_fromKeyPointI, 1); i <= _toKeyPointI; ++i)
	{
		drawNode->setColor(Color3B(Color4F(1.0, 1.0, 1.0, 1.0)));
		drawNode->drawLine(_hillKeyPoints[i - 1], _hillKeyPoints[i],Color4F(drawNode->getColor()));

		// sooth line 
		drawNode->setColor(Color3B(Color4F(1, 0, 0.3, 1)));
		Vec2 p0 = _hillKeyPoints[i - 1];
		Vec2 p1 = _hillKeyPoints[i];
		int hSegment = (p1.x - p0.x) / kHillSegmentWidth;
		float dx = (p1.x - p0.x) / hSegment;
		float da = M_PI / hSegment;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;
		Vec2 pt0, pt1;
		pt0 = p0;
		for (int j = 0; j < hSegment + 1; ++j)
		{
			pt1.x = p0.x + dx*j;
			pt1.y = ymid + ampl*cosf(da*j);
			drawNode->drawLine(pt0, pt1,Color4F(drawNode->getColor()));
			pt0 = pt1;
		}
	}
}

void ::Terrain::generateHills()
{
	float minDX = 160;
	int rangeDX = 80; // from 160 to 240
	float minDY = 60;
	int rangeDY = 40; // from 60 to 100

	float x = -minDX;
	float y = winSize.height / 2;

	float dy, ny;

	float sign = 1; // +1: going up, -1: going down
	float paddingTop = 20;
	float paddingBottom = 20;

	for (int i = 0; i < kMaxHillKeyPoints; ++i)
	{
		_hillKeyPoints[i] = Point(x, y);
		if (i == 0)
		{
			x = 0;
			y = winSize.height / 2;
		}
		else
		{
			x += rand() % rangeDX + minDX;
			while (true)
			{
				dy = rand() % rangeDY + minDY;
				ny = y + dy*sign;
				if ((sign<0 && (ny + minDY)>(winSize.height - paddingTop))
					|| (sign>0 && (ny - minDY) <paddingBottom))
					continue;
				if (ny<(winSize.height - paddingTop) && ny >paddingBottom)
					break;
			}
			y = ny;
		}
		sign *= -1;
	}
}