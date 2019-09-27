#pragma once


class WINDOW
{
	static WINDOW* instance;

	HWND hWnd;
	WNDCLASSEX wc;
	RECT wr;			// calculate the size of the client area

public:
	~WINDOW();

	static WINDOW* getInstance();
	BOOL InitInstance(HINSTANCE, LPCWSTR, int);

private:
	WINDOW();
	BOOL SetClientArea(int, int, int, int);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
