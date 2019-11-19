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

	bool Initialize(WCHAR*, int, int);
	void Shutdown();
	bool Render(RNDMATRIXS& matrixs, int, int);

private:
	bool Update(int, int);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	UINT screenWidth;
	UINT screenHeight;

	LOADIMGFILE* ImgFile;
	TEXTURESHADER* texture;

	int m_bitmapWidth;
	int m_bitmapHeight;
	int m_previousPosX;
	int m_previousPosY;
};