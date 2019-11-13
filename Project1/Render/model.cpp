#include "../Utility/renafx.h"

#include "../Render/loadObjFile.h"
#include "../Render/textureshader.h"
#include "../Lighting/lightclass.h"
#include "../Lighting/lightshader.h"

#include "model.h"



MODEL::MODEL()
	: textureShader(nullptr)
	, light(nullptr), lightShader(nullptr)
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


bool MODEL::Initialize(char* modelFilename, WCHAR* textureFilename, LIGHT_TYPE lightType)
{
	ISFAIL(Load(modelFilename));

	textureShader = new TEXTURESHADER(hwnd, device, deviceContext);
	ISINSTANCE(textureShader);
	if (!textureShader->Initialize(textureFilename))
	{
		ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
	}


	lightShader = new LIGHTSHADER(hwnd, device, deviceContext);
	ISINSTANCE(lightShader);
	if (!lightShader->Initialize(lightType))
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}

	light = new LIGHT;
	ISINSTANCE(light);

	switch (lightType)
	{
	case LIGHT_NONE:
	case LIGHT_DIRECTION:
		light->SetDirectionLight();
		break;
	case LIGHT_POINTLIGHT:
		light->SetPointLight(global.position);
		break;
	default:
		return false;
	}
	return true;
}


bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos)
{
	file->RenderBuffers();

	ISFAIL(textureShader->Render(file->GetIndexCount(), renderMatrix));

	//if (parent) {
	//	ISFAIL(lightShader->Render(file->GetIndexCount(), renderMatrix
	//		, parent->GetPosition(), textureShader->GetTexture(), light->GetLight()));
	//	return true;
	//}

	//ISFAIL(lightShader->Render(file->GetIndexCount(), renderMatrix
	//	, cameraPos, textureShader->GetTexture(), light->GetLight()));
	return true;
}
void MODEL::Shutdown()
{
	SAFE_DELETE(light);
	SAFE_DELETE(lightShader);
	SAFE_DELETE(textureShader);
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
