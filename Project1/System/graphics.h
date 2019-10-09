#pragma once

class CAMERA;
class MODEL;
class COLORSHADER;

#define MODELCOUNT 2


class GRAPHICS : public SINGLETON <GRAPHICS>
{
public:
	GRAPHICS();
	~GRAPHICS();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();

private:
	BOOL Render(RNDMATRIXS&, float);

private:
	RENDER_T* transformation;

	CAMERA * mainCamera;
	MODEL* sphere;
	MODEL* moon;
};

