#pragma once

class LOADOBJECTSFILE;
class COLORSHADER;
class TEXTURESHADER;


struct TRANSFORM
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
};


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
	TEXTURESHADER* textureShader;


//==============================

private:
	TRANSFORM transform;

public:
	bool IsRotation(D3DXMATRIX&, RotationDirection, float);
	bool IsOrbit(D3DXMATRIX&, D3DXVECTOR3, float);
};

