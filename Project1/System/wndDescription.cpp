
#include "../stdafx.h"
#include "wndDescription.h"


WNDDesc::WNDDesc()
	: vsync(true), fullscreen(false)
	, screenWidth(800), screenHeight(600)
	, screenNear(0.1f), screenDepth(1000.f)
{
}

WNDDesc::~WNDDesc()
{
}

void WNDDesc::ResizeScene(UINT width, UINT height)
{
	if (width < 800 || height < 600) return;

	screenWidth = width;
	screenHeight = height;

	//DeleteBackBuffer();
	//{
	//	HRESULT hr = swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);
	//	assert(SUCCEEDED(hr));
	//}
	//CreateBackBuffer(width, height);
}