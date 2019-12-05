#pragma once


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;


class PANEL;
class CameraControl;

class LIGHT;
class LIGHTSHADER;

class RenderTextureClass;
class DepthShaderClass;
class ShadowShaderClass;

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

	void WindowHierarchy();

private:
	RENDER_T* transformation;
	CameraControl * mainCamera;
	LIGHT* light;
	LIGHTSHADER* shader;
	
	
	PANEL* panel;


	MODEL* m_GroundModel;
	RenderTextureClass* m_RenderTexture;
	DepthShaderClass*   m_DepthShader;
	ShadowShaderClass*  m_ShadowShader;
	bool RenderSceneToTexture();



	float rotation;
	MODEL* sphere01;
	MODEL* sphere02;
	MODEL* sphere03;

};
