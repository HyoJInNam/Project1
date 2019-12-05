#pragma once


class LOADOBJECTSFILE;

class BUMPMAPPING;
class BumpMapShaderClass;
class SpecMapShaderClass;
class DepthShaderClass;
class ShadowShaderClass;

class COLORSHADER;
class LIGHT;
class LIGHTSHADER;

class MODEL: public Transform <MODEL>
{
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

public:
	MODEL(const char* name);
	MODEL(const MODEL&);
	~MODEL();
	virtual bool ViewTransform() override;

	bool Initialize(
		char* modelFilename,
		WCHAR* texture = nullptr,
		WCHAR* normalmap = nullptr,
		WCHAR* specularmap = nullptr);


	bool Render(RNDMATRIXS&, D3DXVECTOR3, LIGHT* light);
	bool RenderShadow(RNDMATRIXS&, ID3D11ShaderResourceView*, LIGHT* light);
	bool RenderDepth(RNDMATRIXS&);
	void Shutdown();
	

	bool LoadTextures(
		WCHAR* texture = nullptr,
		WCHAR* normalmap = nullptr,
		WCHAR* specularmap = nullptr)
	{
		return	file->LoadTexture(device, texture, normalmap, specularmap);
	}
	int GetIndexCount() { return file->GetIndexCount(); }
	ID3D11ShaderResourceView* GetTexture() { return file->GetTexture(); }
	LightBufferType* GetLight();


private:
	LOADOBJECTSFILE* file;
	COLORSHADER* material;

	LIGHT* obj_light;
	LIGHTSHADER* shader;

	BumpMapShaderClass* m_BumpMapShader;
	SpecMapShaderClass* m_SpecMapShader;
	DepthShaderClass*   m_DepthShader;
	ShadowShaderClass*  m_ShadowShader;
};