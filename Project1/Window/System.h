#pragma once

class InputClass;
class GraphicsClass;

class SYSTEM : public SINGLETON <SYSTEM>
{
public:
	SYSTEM();
	~SYSTEM();

	BOOL Initialize();
	BOOL Frame();
	BOOL Render();
	void Shutdown();
	IMGUI_API LRESULT  SystemProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
};
