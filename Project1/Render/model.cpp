#include "renafx.h"
#include "model.h"



MODEL::MODEL()
	: colorShader(nullptr)
	, texture(nullptr), textureShader(nullptr)
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();

	file = new LOADOBJECTSFILE(device, deviceContext);
}


MODEL::MODEL(const MODEL& other) {}
MODEL::~MODEL() 
{
	SAFE_DELETE(file);
}


bool MODEL::Initialize(char* modelFilename, WCHAR* textureFilename)
{
	ISFAIL(Load(modelFilename));

	if (!textureFilename) {
		colorShader = new COLORSHADER(hwnd, device, deviceContext);
		ISINSTANCE(colorShader);
		if (!colorShader->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the color shader object.", L"ERROR");
			return false;
		}
	}

	texture = new TEXTURE(device);
	ISINSTANCE(texture);
	if (!texture->Initialize(textureFilename))
	{
		ERR_MESSAGE(L"Could not initialize the texture object.", L"ERROR");
	}

	textureShader = new TEXTURESHADER(hwnd, device, deviceContext);
	ISINSTANCE(textureShader);
	if (!textureShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
	}
	return true;
}
bool MODEL::Render(RNDMATRIXS renderMatrix)
{
	file->RenderBuffers();
	if(colorShader != nullptr)
		ISFAIL(colorShader->Render(file->GetIndexCount(), renderMatrix));
	
	ISFAIL(textureShader->Render(file->GetIndexCount(), renderMatrix, texture->GetTexture()));
	return true;
}
void MODEL::Shutdown()
{
	SAFE_DELETE(textureShader);
	SAFE_DELETE(texture);
	SAFE_DELETE(colorShader);
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