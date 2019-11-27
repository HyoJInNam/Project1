#pragma once

class Camera;
class CameraPanelControler;
class BitmapClass;

class PANEL
{
public:
	PANEL();
	~PANEL();

	BOOL Initialize();
	BOOL Frame();
	BOOL Render(RNDMATRIXS& transformation);
	void Shutdown();

	void ShowWindowHierarchy();

private:
	RENDER_T* transformation;
	CameraPanelControler * PanelCamera;

	BitmapClass* bitmap;
};