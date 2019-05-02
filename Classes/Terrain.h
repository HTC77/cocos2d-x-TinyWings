#ifndef __TinyWings__Terrain__
#define __TinyWings__Terrain__

#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 10

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

	int _fromKeyPointI;
	int _toKeyPointI;
	Vec2 _hillKeyPoints[kMaxHillKeyPoints];
    CREATE_FUNC(Terrain);

};

#endif // __TinyWings__Terrain__