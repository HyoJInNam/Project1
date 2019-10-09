#pragma once

class LOADOBJECTSFILE;
class COLORSHADER;
class TEXTURE;
class TEXTURESHADER;

class MODEL
{
public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(char*, WCHAR*);
	void Shutdown();
	bool Render(RNDMATRIXS );
	
private:
	bool Load(char*);

private:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	LOADOBJECTSFILE* file;
	COLORSHADER * colorShader;
	TEXTURE* texture;
	TEXTURESHADER* textureShader;
};

