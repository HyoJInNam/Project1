
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

bool MODEL::Initialize(char* modelFilename, WCHAR* texture_file_name, WCHAR* normal_texture_name, WCHAR* filename3)
{

	ISFAIL(file->Initialize(modelFilename));
	file->LoadTexture(device, texture_file_name, normal_texture_name, filename3);

	light = new LIGHT;
	ISINSTANCE(light);
	light->SetDirectionLight();
	
	if (filename3)
	{
		m_SpecMapShader = new SpecMapShaderClass(hwnd, device, deviceContext);
		ISINSTANCE(m_SpecMapShader);
		if (!m_SpecMapShader->Initialize())
		{
			MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (normal_texture_name)
	{
		m_BumpMapShader = new BumpMapShaderClass(hwnd, device, deviceContext);
		ISINSTANCE(m_BumpMapShader);
		if (!m_BumpMapShader->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the Bump Map shader object.", L"ERROR");
			return false;
		}
	}

	shader = new LIGHTSHADER(hwnd, device, deviceContext);
	if (!shader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}
	
	return true;
}

bool MODEL::LoadTextures(WCHAR* texture_file_name, WCHAR* normal_texture_name, WCHAR* filename3)
{
	return	file->LoadTexture(device, texture_file_name, normal_texture_name, filename3);
}

bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos)
{
	file->Render();
	if (m_BumpMapShader)
	{
		m_BumpMapShader->Render(file->GetIndexCount(), renderMatrix, file->GetTextures(), light->GetDirection(), light->GetDiffuseColor());
	}

	if (m_SpecMapShader)
	{
		m_SpecMapShader->Render(file->GetIndexCount(), renderMatrix,
			file->GetTextures(), light->GetDirection(), light->GetDiffuseColor(),
			cameraPos, light->GetSpecularColor(), light->GetSpecularPower());
	}

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

	SAFE_DELETE(shader);
	SAFE_DELETE(shader);
	SAFE_DELETE(material);

	file->Shutdown();
	return;
}
