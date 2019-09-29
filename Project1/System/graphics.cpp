#include "../stdafx.h"
#include "render.h"
#include "graphics.h"


GRAPHICS::GRAPHICS() {}
GRAPHICS::~GRAPHICS() {}
BOOL GRAPHICS::Initialize()
{
	return 0;
}

void GRAPHICS::Shutdown()
{
}


BOOL GRAPHICS::Frame()
{
	return Render();
}

BOOL GRAPHICS::Render()
{
	D3D::GetInstance()->BeginScene(D3DXCOLOR(225.0f, 225.0f, 225.0f, 1.0f));

	D3D::GetInstance()->EndScene();
	return true;
}
