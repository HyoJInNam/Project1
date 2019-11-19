#include "../../Utility/renafx.h"

#include "../Files/loadFile.h"
#include "../Files/LoadImgFile.h"

#include"../Shader/textureshader.h"
#include "BitmapClass.h"


BitmapClass::BitmapClass()
	: m_bitmapWidth(0), m_bitmapHeight(0)
	, m_previousPosX(0), m_previousPosY(0)
{
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();


	ImgFile = new LOADIMGFILE(device, deviceContext);
}

BitmapClass::BitmapClass(const BitmapClass& other) {}

BitmapClass::~BitmapClass() {}


bool BitmapClass::Initialize(WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	WNDDesc::GetInstance()->getSceneSize(screenWidth, screenHeight);

	// 렌더링할 비트맵의 픽셀의 크기를 저장
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

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


bool BitmapClass::Render(RNDMATRIXS& matrixs, int positionX, int positionY)
{
	ImgFile->GetTexture();
	if(!Update(positionX, positionY))
	{
		return false;
	}
	ImgFile->RenderBuffers();
	texture->Render(ImgFile->GetIndexCount(), matrixs, ImgFile->GetTexture());
	return true;
}


bool BitmapClass::Update(int positionX, int positionY)
{
	float left, right, top, bottom;


	// 이 비트맵을 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트 하지 마십시오.
	if((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}
	


	m_previousPosX = positionX;
	m_previousPosY = positionY;


	left = (float)(((float)screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)((float)screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	ImgFile->UpdateBuffers(left / 100, right / 100, top / 100, bottom / 100);
	return true;
}