#pragma once

class CAMERA;
class MODEL;
class COLORSHADER;
class TextClass;

#define MODELCOUNT 9


class GRAPHICS : public SINGLETON <GRAPHICS>
{
public:
	GRAPHICS();
	~GRAPHICS();

	BOOL Initialize();
	BOOL Frame(int fps, int cpu, float frameTime);
	BOOL Render();
	void Shutdown();

private:
	RENDER_T* transformation;
	CAMERA * mainCamera;
	CAMERA * textCamera;
	TextClass* m_Text;

	float rotation;
	MODEL* Solar;
	MODEL* Mercury;
	MODEL* Venus;
	MODEL* Earth;
	MODEL* moon;
};

