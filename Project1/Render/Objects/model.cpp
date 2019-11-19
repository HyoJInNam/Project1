//#include "../../Utility/stdafx.h"
#include "../../Utility/renafx.h"

#include "../Files/LoadFile.h"
#include "../Files/loadObjFile.h"
#include "../Files/TextureArrayClass.h"

#include "../Shader/colorshader.h"
#include "../Shader/BumpMapShaderClass.h"
#include "../Lighting/lightclass.h"
#include "../Lighting/lightshader.h"

#include"BumpMapping.h"
#include "model.h"



MODEL::MODEL()
	: textureShader(nullptr)
	, global{  D3DXVECTOR3(0.0f, 0.0f, 0.0f)
			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)
			, D3DXVECTOR3(1.0f, 1.0f, 1.0f)
			, D3DXVECTOR3(0.0f, 0.0f, 0.0f) }
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();

	file = new LOADOBJECTSFILE(device, deviceContext);
}


MODEL::MODEL(const MODEL& other) {}
MODEL::~MODEL() {}


int  MODEL::GetIndexCount()	{return file->GetIndexCount(); }
ID3D11ShaderResourceView* MODEL::GetTexture() {	return file->GetTexture(); }

bool MODEL::Initialize(char* modelFilename)
{
	//ISFAIL(Load(modelFilename));
	bumpmap = new BUMPMAPPING(device, deviceContext);
	ISINSTANCE(bumpmap);
	if (!bumpmap->Initialize(modelFilename
		, const_cast<WCHAR*>(L"./data/models/stone01.dds")
		, const_cast<WCHAR*>(L"./data/models/bump01.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the BumpMap object.", L"ERROR");
		return false;
	}

	textureShader = new LIGHTSHADER;
	textureShader->SetDirectionLight();
	if (!textureShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}

	m_BumpMapShader = new BumpMapShaderClass;
	ISINSTANCE(m_BumpMapShader);

	if (!m_BumpMapShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the Bump Map shader object.", L"ERROR");
		return false;
	}
	return true;
}
bool MODEL::LoadTexture(WCHAR * filename)
{
	return	file->LoadTexture(filename);
}
bool MODEL::LoadTextures(WCHAR * filename1, WCHAR * filename2)
{
	return	bumpmap->LoadTextures(device, filename1, filename2);
}

bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos)
{
	//file->RenderBuffers();
	bumpmap->Render();
	m_BumpMapShader->Render(bumpmap->GetIndexCount(), renderMatrix, bumpmap->GetTextures(), textureShader->GetDirection(), textureShader->GetDiffuseColor());

	/*if (textureShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture()) == false)
	{
		colorShader = new COLORSHADER(hwnd, device, deviceContext);
		ISINSTANCE(colorShader);
		if (!colorShader->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
		}

		colorShader->Render(file->GetIndexCount(), renderMatrix);
	}*/
	return true;
}
void MODEL::Shutdown()
{
	SAFE_DELETE(m_BumpMapShader);
	bumpmap->Shutdown();
	SAFE_DELETE(textureShader);
	SAFE_DELETE(colorShader);
	file->ReleaseTexture();
	file->ShutdownBuffers();
	return;
}


bool MODEL::Load(char* modelFilename)
{
	CString m_temp = modelFilename;
	CString temp = m_temp.Right(4);
	if (temp.CompareNoCase(_T(".obj")) == 0) { ISFAIL(file->LoadObjFile(modelFilename)); }
	if (temp.CompareNoCase(_T(".txt")) == 0) { ISFAIL(file->LoadTextFile(modelFilename)); }
	
	ISFAIL(file->InitializeBuffers());
	return true;
}
