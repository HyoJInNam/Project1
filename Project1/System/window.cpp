#include "../stdafx.h"
#include "window.h"


WINDOW* WINDOW::instance = NULL;

WINDOW::WINDOW()
{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
}


WINDOW::~WINDOW()
{
	SAFE_DELETE(instance);
}

WINDOW * WINDOW::getInstance()
{
	if (!instance) {
		instance = new WINDOW;
	}
	return instance;
}

BOOL WINDOW::InitInstance(HINSTANCE hInstance, LPCWSTR title, int mCmdShow)
{
	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	SetClientArea(0, 0, 500, 400);
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		title,    // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		abs(wr.right - wr.left),    // width of the window
		abs(wr.bottom - wr.top),    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		wc.hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	ShowWindow(hWnd, mCmdShow);
	return true;
}
BOOL WINDOW::SetClientArea(int bottom, int left, int right, int top)
{
	wr.left = left;
	wr.top = top;
	wr.right = right;
	wr.bottom = bottom;

	return true;
}



LRESULT CALLBACK WINDOW::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}