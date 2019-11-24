#include <stdafx.h>
#include "Window/window.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	WINDOW* wnd = new WINDOW(hInstance, nCmdShow);
	ISINSTANCE(wnd);

	wnd->InitInstance(L"dirextX");
	WPARAM wParam = wnd->Run();

	wnd->Shutdown();
	SAFE_DELETE(wnd);
	
	return wParam;
}