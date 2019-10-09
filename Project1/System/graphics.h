#pragma once



class CAMERA;
class MODEL;
class COLORSHADER;

class GRAPHICS : public SINGLETON <GRAPHICS>
{
public:
	GRAPHICS();
	~GRAPHICS();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();

private:
	BOOL Render(float);

private:
	RENDER_T* transformation;

	CAMERA * mainCamera;
	MODEL * cube;
};

