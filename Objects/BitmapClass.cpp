
#include <objectstdafx.h>
#include "BitmapClass.h"

#define PERSENT 100.0f

BitmapClass::BitmapClass()
	: m_bitmapWidth(0), m_bitmapHeight(0)
	, posX(0), posY(0)
{
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();


	ImgFile = new LOADIMGFILE(device, deviceContext);
}

BitmapClass::BitmapClass(const BitmapClass& other) {}

BitmapClass::~BitmapClass() {}


bool BitmapClass::Initialize(WCHAR* textureFilename)
{
	WNDDesc::GetInstance()->getSceneSize(screenWidth, screenHeight);

	HRESULT result;
	D3DX11_IMAGE_INFO imageInfo;
	D3DX11GetImageInfoFromFile(textureFilename, NULL, &imageInfo, &result);
	ISFAILED(result);

	m_bitmapWidth = imageInfo.Width + 1.0f;
	m_bitmapHeight = imageInfo.Height + 1.0f;

	//¿øÁ¡
	left = ((float)m_bitmapWidth / -2.0f);
	right = left + ((float)m_bitmapWidth);
	bottom = ((float)m_bitmapHeight / -2.0f);
	top = bottom + ((float)m_bitmapHeight);

	posX = 0;
	posY = 0;

	if (!ImgFile->InitializeBuffers())
	{
		return false;
	}
	ImgFile->LoadTexture(textureFilename);
	
	texture = new TEXTURESHADER(WNDDesc::GetInstance()->getHwnd(), device, deviceContext);
	ISINSTANCE(texture)
	if (!texture->Initialize())
	{
		return false;
	}

	return true;
}


void BitmapClass::Shutdown()
{
	ImgFile->ReleaseTexture();
	ImgFile->ShutdownBuffers();
	texture->Shutdown();
}


bool BitmapClass::Render(RNDMATRIXS& matrixs)
{
	////ImgFile->GetTexture();	
	ImgFile->UpdateBuffers(((left / 100) + posX), ((right / 100) + posX)
						, ((top / 100) + posY), ((bottom / 100) + posY));

	ImgFile->RenderBuffers();
	texture->Render(ImgFile->GetIndexCount(), matrixs, ImgFile->GetTexture());
	return true;
}


bool BitmapClass::Reposition(float positionX, float positionY)
{
	if ((positionX == posX) && (positionY == posY))
	{
		return true;
	}

	posX = positionX;
	posY = positionY;
	return true;
}


bool BitmapClass::Resize(float width, float height)
{
	if ((width == m_bitmapWidth) && (height == m_bitmapHeight))
	{
		return true;
	}

	if ((!width) && (!height)) return true;
	if (!width) m_bitmapHeight = height;
	else if (!height) m_bitmapWidth = width;
	else {
		m_bitmapWidth = width;
		m_bitmapHeight = height;
	}

	left   = ((float)m_bitmapWidth / -2.0f);
	right  = left + ((float)m_bitmapWidth);
	bottom = ((float)m_bitmapHeight / -2.0f);
	top    = bottom + ((float)m_bitmapHeight);

	return true;
}
