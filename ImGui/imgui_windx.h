#pragma once

#include <Utility.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"



struct ID3D11Device;
struct ID3D11DeviceContext;

class WINDX_IMGUI :public SINGLETON <WINDX_IMGUI>
{
	HWND                 g_hWnd = 0;
	INT64                g_Time = 0;
	INT64                g_TicksPerSecond = 0;
	ImGuiMouseCursor     g_LastMouseCursor = ImGuiMouseCursor_COUNT;

public:
	WINDX_IMGUI() {};
	WINDX_IMGUI(const WINDX_IMGUI&) {};
	~WINDX_IMGUI() {};

	IMGUI_API bool Initialize(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	IMGUI_API void Frame();
	IMGUI_API void Render();
	IMGUI_API void Shutdown();

	IMGUI_API bool UpdateMouseCursor();
	IMGUI_API void UpdateMousePos();
	IMGUI_API LRESULT  ImguiProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

};