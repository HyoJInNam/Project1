#pragma once


class LOADOBJECTSFILE;

class BUMPMAPPING;
class BumpMapShaderClass;
class SpecMapShaderClass;
class DepthShaderClass;
class ShadowShaderClass;

class LIGHTSHADER;
class COLORSHADER;
class LIGHT;

enum renderMode {
	Ink,
	SpecBump,
	ToonBump,
	Bump,
	Toon,
};
class MODEL: public Transform <MODEL>
{

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

	renderMode rmode;
	bool isSpin = false;
private:
	LOADOBJECTSFILE* file;
	COLORSHADER* material;

	LIGHT* light;
	LIGHTSHADER* ToonColorShader;
	BumpMapShaderClass* m_BumpMapShader;
	BumpMapShaderClass* m_ToonShader;
	SpecMapShaderClass* m_SpecMapShader;
	SpecMapShaderClass* m_InkShader;

	DepthShaderClass*   m_DepthShader;
	ShadowShaderClass*  m_ShadowShader;
};