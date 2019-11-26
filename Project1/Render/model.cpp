
#include <stdafx.h>
#include "model.h"



MODEL::MODEL()
	: shader(nullptr)
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

bool MODEL::Initialize(char* modelFilename, WCHAR* filename)
{
	ISFAIL(Load(modelFilename));
	file->LoadTexture(filename);
	// specular map shader 객체를 생성한다.
	
	light = new LIGHT;
	ISINSTANCE(light);
	light->SetDirectionLight();

	shader = new LIGHTSHADER;
	if (!shader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}

	return true;
}
bool MODEL::Initialize(char* modelFilename, WCHAR* filename1, WCHAR* filename2)
{
	//ISFAIL(Load(modelFilename));
	bumpmap = new BUMPMAPPING(device, deviceContext);
	ISINSTANCE(bumpmap);
	if (!bumpmap->Initialize(modelFilename, filename1, filename2))
	{
		ERR_MESSAGE(L"Could not initialize the BumpMap object.", L"ERROR");
		return false;
	}

	light = new LIGHT;
	ISINSTANCE(light);
	light->SetDirectionLight();

	shader = new LIGHTSHADER;
	if (!shader->Initialize())
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
bool MODEL::Initialize(char* modelFilename, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	//ISFAIL(Load(modelFilename));
	bumpmap = new BUMPMAPPING(device, deviceContext);
	ISINSTANCE(bumpmap);
	if (!bumpmap->Initialize(modelFilename, filename1, filename2, filename3))
	{
		ERR_MESSAGE(L"Could not initialize the BumpMap object.", L"ERROR");
		return false;
	}


	light = new LIGHT;
	ISINSTANCE(light);
	light->SetDirectionLight();

	shader = new LIGHTSHADER;
	if (!shader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}

	m_SpecMapShader = new SpecMapShaderClass;
	ISINSTANCE(m_SpecMapShader);
	if (!m_SpecMapShader->Initialize(device, hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
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
	if (m_BumpMapShader)
	{
		bumpmap->Render();
		m_BumpMapShader->Render(bumpmap->GetIndexCount(), renderMatrix, bumpmap->GetTextures(), light->GetDirection(), light->GetDiffuseColor());

		return true;
	}

	if (m_SpecMapShader)
	{
		bumpmap->Render();
		m_SpecMapShader->Render(deviceContext, bumpmap->GetIndexCount(), renderMatrix,
			bumpmap->GetTextures(), light->GetDirection(), light->GetDiffuseColor(),
			cameraPos, light->GetSpecularColor(), light->GetSpecularPower());

		return true;
	}

	file->RenderBuffers();
	if (shader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture(), light->GetLight()) == false)
	{
		material = new COLORSHADER(hwnd, device, deviceContext);
		ISINSTANCE(material);
		if (!material->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
		}

		material->Render(file->GetIndexCount(), renderMatrix);
	}
	return true;
}
void MODEL::Shutdown()
{
	if (m_SpecMapShader)  m_SpecMapShader->Shutdown();
	if (m_BumpMapShader) SAFE_DELETE(m_BumpMapShader);
	if (bumpmap) bumpmap->Shutdown();

	SAFE_DELETE(shader);
	SAFE_DELETE(shader);
	SAFE_DELETE(material);
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
