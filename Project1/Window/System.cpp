#include <stdafx.h>
#include "graphics.h"
#include "System.h"


SYSTEM::SYSTEM() {}
SYSTEM::~SYSTEM() {}

BOOL SYSTEM::Initialize()
{
	Time::GetInstance()->Start();
	INPUTS::GetInstance()->Initialize();
	GRAPHICS::GetInstance()->Initialize();
	WINDX_IMGUI::GetInstance()->Initialize(WNDDesc::GetInstance()->getHwnd(),
						D3D::GetInstance()->GetDevice(),
						D3D::GetInstance()->GetDeviceContext());
	return true;
}

BOOL SYSTEM::Frame()
{

	Time::GetInstance()->Update();
	if (ImGui::IsAnyWindowHovered() == false)
		ISFAIL(INPUTS::GetInstance()->Frame());
	
	WINDX_IMGUI::GetInstance()->Frame();
	ISFAIL(GRAPHICS::GetInstance()->Frame());

	return true;
}

BOOL SYSTEM::Render()
{
	//D3D* d3d = D3D::GetInstance();
	//d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	GRAPHICS::GetInstance()->Render();
	//WINDX_IMGUI::GetInstance()->Render();

	//d3d->EndScene();
	return true;
}

void SYSTEM::Shutdown()
{
	GRAPHICS::GetInstance()->Shutdown();
	WINDX_IMGUI::GetInstance()->Shutdown();
	INPUTS::GetInstance()->Shutdown();
	Time::GetInstance()->destroy();
}

IMGUI_API LRESULT SYSTEM::SystemProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return IMGUI_API WINDX_IMGUI::GetInstance()->ImguiProcHandler(hWnd, msg, wParam, lParam);
}