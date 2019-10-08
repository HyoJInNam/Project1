#pragma once


class WNDDesc : public SINGLETON <WNDDesc>
{
public:
	WNDDesc();
	~WNDDesc();

	void setHwnd(HWND Handle) { this->Handle = Handle; }
	HWND getHwnd() { return this->Handle; }

	void getSceneSize(UINT&, UINT& );
	void getSceneDepth(FLOAT&, FLOAT&);
	void ResizeScene(UINT, UINT);


private:
	HWND Handle;

	BOOL vsync;
	BOOL fullscreen;

	UINT sceneWidth;
	UINT sceneHeight;
	FLOAT sceneDepth;
	FLOAT sceneNear;

	friend class D3D;
};

