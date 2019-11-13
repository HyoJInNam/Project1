#pragma once

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

private:
	RENDER_T* transformation;
	CameraControl * mainCamera;
	LIGHTSHADER* light;
	LIGHTSHADER* pointLight;

	float rotation;
	MODEL* Solar;
	MODEL* Mercury;
	MODEL* Venus;
	MODEL* Earth;
	MODEL* moon;
};

