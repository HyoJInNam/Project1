#pragma once


class WINDOW
{
public:
	WINDOW(HINSTANCE hInstance, int nCmdShow);
	~WINDOW();

	BOOL InitInstance(LPCWSTR);
	BOOL Run();
	void Shutdown();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE instance;
	int wndCmdShow;
};
