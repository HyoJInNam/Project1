#pragma once

class CAMERA;
class MODEL;
class COLORSHADER;

#define MODELCOUNT 9


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

	MODEL* Solar;
	MODEL* Mercury;
	MODEL* Venus;
	MODEL* Earth;
	//MODEL* moon;
	MODEL* sphere [MODELCOUNT] ;
};

