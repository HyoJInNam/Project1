#pragma once


class TEXTURESHADER;
class LOADIMGFILE;

class BitmapClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3  position;
		D3DXVECTOR2  texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(WCHAR*);
	void Shutdown();
	bool Render(RNDMATRIXS& matrixs);

	bool Reposition(float, float);
	bool Resize(float, float);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	UINT screenWidth;
	UINT screenHeight;

	LOADIMGFILE* ImgFile;
	TEXTURESHADER* texture;

	float m_bitmapWidth;
	float m_bitmapHeight;
	float posX;
	float posY;

	float left, right, top, bottom;
};