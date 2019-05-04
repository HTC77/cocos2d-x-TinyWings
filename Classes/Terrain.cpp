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
	_stripes = new Sprite();
	this->addChild(drawNode);
	this->generateHills();
	this->resetHillVertices();
	this->setGLProgram(
		GLProgramCache::getInstance()->getGLProgram(
			GLProgram::SHADER_NAME_POSITION_TEXTURE));

	_batchNode = SpriteBatchNode::create("TinySeal.png");
	this->addChild(_batchNode);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TinySeal.plist");

    return true;
}

void ::Terrain::resetHillVertices()
{
	static int prevFromKeyPointI = -1;
	static int prevToKeyPointI = -1;

	// key point interval for drawing
	while (_hillKeyPoints[_fromKeyPointI + 1].x < _offsetX - winSize.width / 8 / this->getScale())
	{
		_fromKeyPointI++;
	}
	while (_hillKeyPoints[_toKeyPointI].x < _offsetX + winSize.width * 12 / 8 / this->getScale())
	{
		_toKeyPointI++;
	}

	float minY = 0;

	if (prevFromKeyPointI != _fromKeyPointI || prevToKeyPointI != _toKeyPointI)
	{
		_nHillVertices = 0;
		_nBorderVertices = 0;
		Vec2 p0, p1, pt0, pt1;
		p0 = _hillKeyPoints[_fromKeyPointI];
		for (int i = _fromKeyPointI + 1; i < _toKeyPointI + 1; ++i)
		{
			p1 = _hillKeyPoints[i];
			int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);
			float dx = (p1.x - p0.x) / hSegments;
			float da = M_PI / hSegments;
			float ymid = (p0.y + p1.y) / 2;
			float ampl = (p0.y - p1.y) / 2;
			pt0 = p0;
			_borderVertices[_nBorderVertices++] = pt0;
			for (int j = 0; j < hSegments + 1; ++j)
			{
				pt1.x = p0.x + dx*j;
				pt1.y = ymid + ampl*cosf(j*da);
				_borderVertices[_nBorderVertices++] = pt1;

				_hillVertices[_nHillVertices] = Point(pt0.x, 0 + minY);
				_hillTexCoords[_nHillVertices++] = Point(pt0.x / 512, 1.0f);

				_hillVertices[_nHillVertices] = Point(pt1.x, 0 + minY);
				_hillTexCoords[_nHillVertices++] = Point(pt1.x / 512, 1.0f);

				_hillVertices[_nHillVertices] = Point(pt0.x, pt1.y);
				_hillTexCoords[_nHillVertices++] = Point(pt0.x / 512, 0);

				_hillVertices[_nHillVertices] = Point(pt1.x, pt1.y);
				_hillTexCoords[_nHillVertices++] = Point(pt1.x / 512, 0);

				pt0 = pt1;
			}
			p0 = p1;
		}
		prevFromKeyPointI = _fromKeyPointI;
		prevToKeyPointI = _toKeyPointI;
		this->resetPhysicsBody();
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
	CC_NODE_DRAW_SETUP();
	GL::bindTexture2D(_stripes->getTexture()->getName());
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION |
		GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2,
		GL_FLOAT, GL_FALSE, 0, _hillVertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2,
		GL_FLOAT, GL_TRUE, 0, _hillTexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(_nHillVertices));
}

void ::Terrain::resetPhysicsBody()
{
	if (_body) this->removeChild(_body);
	_body = Sprite::create();
	for (int i = 0; i < _nBorderVertices - 1; i+=4)
	{
		auto ep1 = Vec2(_borderVertices[i].x, _borderVertices[i].y);
		auto ep2 = Vec2(_borderVertices[i + 1].x, _borderVertices[i + 1].y);
		auto edgeSeg = PhysicsBody::createEdgeSegment(ep1, ep2);
		auto edgeNode = Node::create();
		edgeNode->setPhysicsBody(edgeSeg);
		_body->addChild(edgeNode);
	}
	this->addChild(_body);
}

void ::Terrain::setOffsetX(float newOffsetX)
{
	_offsetX = newOffsetX;
	this->setPosition(Point(winSize.width / 12 - _offsetX * this->getScale(), 0));
	this->resetHillVertices();
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
