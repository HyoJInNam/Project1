#pragma once


class LOADOBJECTSFILE;

class BUMPMAPPING;
class BumpMapShaderClass;
class SpecMapShaderClass;

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
	void Shutdown();
	

	bool LoadTextures(
		WCHAR* texture = nullptr,
		WCHAR* normalmap = nullptr,
		WCHAR* specularmap = nullptr);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();




private:
	LOADOBJECTSFILE* file;
	COLORSHADER* material;

	LIGHT* obj_light;
	LIGHTSHADER* shader;

	BumpMapShaderClass* m_BumpMapShader;
	SpecMapShaderClass* m_SpecMapShader = nullptr;
};