#ifndef __TinyWings__Terrain__
#define __TinyWings__Terrain__

#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 2
#define kMaxHillVertices 4000
#define kMaxBorderVertices 800
#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Terrain : public Node
{
public:

    virtual bool init();
    
    Size winSize;
	void generateHills();
	void resetHillVertices();
	void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	void onDrawHills();
	DrawNode* drawNode;
	CustomCommand _customCommand;
	CREATE_FUNC(Terrain);

	int _fromKeyPointI;
	int _toKeyPointI;
	Vec2 _hillKeyPoints[kMaxHillKeyPoints];

	int _nBorderVertices;
	int _nHillVertices;
	Vec2 _hillVertices[kMaxHillVertices];
	Vec2 _hillTexCoords[kMaxHillVertices];
	Vec2 _borderVertices[kMaxBorderVertices];
	Sprite* _stripes;
};

#endif // __TinyWings__Terrain__