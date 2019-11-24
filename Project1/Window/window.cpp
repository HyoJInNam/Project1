#include <stdafx.h>
#include "../resource.h"
#include "System.h"
#include "window.h"


WINDOW::WINDOW(HINSTANCE hInstance, int nCmdShow)
	: instance(hInstance)
	, wndCmdShow(nCmdShow)
{
}

WINDOW::~WINDOW() {}

BOOL WINDOW::InitInstance(LPCWSTR title)
{
	if (!title) title = L"Project1";
	WNDCLASSEX wc;
	{
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = instance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = title;
	}
	RegisterClassEx(&wc);

	HWND Handle = CreateWindowEx(NULL,
		title,					  // name of the window class
		title,				      // title of the window
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,			  // x-position of the window
		CW_USEDEFAULT,			  // y-position of the window
		CW_USEDEFAULT,			  // width of the window
		CW_USEDEFAULT,			  // height of the window
		NULL,					  // we have no parent window, NULL
		NULL,					  // we aren't using menus, NULL
		instance,			      // application handle
		NULL);					  // used with multiple windows, NULL

	assert(Handle != NULL);
	WNDDesc::GetInstance()->setHwnd(Handle);
	ShowWindow(Handle, wndCmdShow);
	ShowCursor(true);

	SYSTEM::GetInstance()->Initialize();
	return true;
}

BOOL WINDOW::Run()
{
	MSG msg;
	bool done = false;

	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) done = true;
		if (!SYSTEM::GetInstance()->Frame()) done = true;
	}

	return true;
}

void WINDOW::Shutdown()
{
	SYSTEM::GetInstance()->Shutdown();
}



LRESULT CALLBACK WINDOW::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_SIZE:
		{
			UINT Width = (UINT)LOWORD(lParam);
			UINT Height = (UINT)HIWORD(lParam);
			WNDDesc::GetInstance()->ResizeScene(Width, Height);
			break;
		}

		case WM_DESTROY:
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}