#pragma once


class WINDOW
{
public:
	WINDOW(HINSTANCE hInstance, int nCmdShow);
	~WINDOW();

	BOOL InitInstance(LPCWSTR, UINT, UINT);
	BOOL Run();
	void Shutdown();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	WNDDesc wnd;
	HINSTANCE instance;
	int wndCmdShow;
};
