#pragma once


class WNDDesc : public SINGLETON <WNDDesc>
{
public:
	WNDDesc();
	~WNDDesc();

	void setHwnd(HWND Handle) { this->Handle = Handle; }
	HWND getHwnd() { return this->Handle; }
	void ResizeScene(UINT, UINT);


private:
	HWND Handle;

	BOOL vsync;
	BOOL fullscreen;

	UINT screenWidth;
	UINT screenHeight;
	FLOAT screenDepth;
	FLOAT screenNear;


	friend class D3D;
};

