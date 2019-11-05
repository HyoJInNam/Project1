#pragma once

class LOADOBJECTSFILE;
class COLORSHADER;
class TEXTURESHADER;

enum PIVOT {LOCAL, GLOBAL};

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

	void TransformScale(float, float, float);
	void TransformPosition(float, float, float);
	void TransformRotation(float, float, float);

	void TransformScale(D3DXVECTOR3);
	void TransformPosition(D3DXVECTOR3);
	void TransformRotation(D3DXVECTOR3);

	void TransformMatrix(TRANSFORM&);
	bool Render(PIVOT, RNDMATRIXS& );


	
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
	TRANSFORM local;

public:
};

