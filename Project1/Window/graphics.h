#pragma once

class PANEL;
class CameraControl;

class LIGHT;
class LIGHTSHADER;
class MODEL;


class GRAPHICS : public SINGLETON <GRAPHICS>
{
public:
	GRAPHICS();
	~GRAPHICS();

	BOOL Initialize();
	BOOL Frame();
	BOOL Render();
	void Shutdown();

	void ViewTransform(MODEL* object);

private:
	RENDER_T* transformation;
	CameraControl * mainCamera;
	LIGHT* light;
	LIGHTSHADER* shader;

	PANEL* panel;

	float rotation;
	MODEL* model;
	MODEL* sphere;
	MODEL* sphere2;

};
