
#include "./stdafx.h"
#include "wndDescription.h"


WNDDesc::WNDDesc()
	: vsync(true), fullscreen(false)
	, sceneWidth(800), sceneHeight(600)
	, sceneNear(0.1f), sceneDepth(1000.f)
{
}

WNDDesc::~WNDDesc() {}

void WNDDesc::getSceneSize(UINT& _width, UINT& _height)
{
	_width = sceneWidth;
	_height = sceneHeight;
}

void WNDDesc::getSceneDepth(FLOAT& _near, FLOAT& _depth)
{
	_near = sceneNear;
	_depth = sceneDepth;
}


void WNDDesc::ResizeScene(UINT& width, UINT& height)
{
	if (width < 800 || height < 600) return;

	sceneWidth = width;
	sceneHeight = height;
}