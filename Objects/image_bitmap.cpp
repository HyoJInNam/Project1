
#include <objectstdafx.h>
#include "image_bitmap.h"

#define PERSENT 100.0f

BitmapClass::BitmapClass(const char* name)
	: Transform(name), m_bitmapWidth(0), m_bitmapHeight(0)
	//, posX(0), posY(0)
{
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();


	ImgFile = new LOADIMGFILE(device, deviceContext);
}

BitmapClass::BitmapClass(const BitmapClass& other) : Transform(other) {}

BitmapClass::~BitmapClass() {}

bool BitmapClass::ViewTransform()
{
	if (show_inspector)
	{
		ImGui::Begin(GetObjectName(), &show_inspector);
		{
			ImGui::Text("transform");
			D3DXVECTOR3 pos = GetPosition();
			ImGui::DragFloat3("position", (float*)&pos, 0.1f, 0, 0);
			SetPosition(pos);

			float width = m_bitmapWidth;
			ImGui::DragFloat("Width", (float*)&width, 1.0f, 0, 0);
			float height = m_bitmapHeight;
			ImGui::DragFloat("Height", (float*)&height, 1.0f, 0, 0);
			Resize(width, height);

			D3DXVECTOR3 rot = GetRotation();
			ImGui::DragFloat3("rotation", (float*)&rot, 0.1f, 0, 0);
			SetRotation(rot);

			D3DXVECTOR3 scl = GetScale();
			ImGui::DragFloat3("scale", (float*)&scl, 0.1f, 0, 0);
			SetScale(scl);

		}
		ImGui::End();
	}
	return true;
}


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

	position.x = 0;
	position.y = 0;

	if (!ImgFile->InitializeBuffers())
	{
		return false;
	}
	ImgFile->LoadTexture(device, textureFilename);
	
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
	ImgFile->UpdateBuffers(((left / 100) + position.x), ((right / 100) + position.x)
						, ((top / 100) + position.y), ((bottom / 100) + position.y));

	ImgFile->RenderBuffers();
	texture->Render(ImgFile->GetIndexCount(), matrixs, ImgFile->GetTexture());
	return ViewTransform();
}


bool BitmapClass::Reposition(float positionX, float positionY)
{
	if ((positionX == position.x) && (positionY == position.y))
	{
		return true;
	}

	position.x = positionX;
	position.y = positionY;
	return true;
}

bool BitmapClass::FullScreen()
{
	return Resize((float)screenWidth*2.0f, (float)screenHeight*2.0f);
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
