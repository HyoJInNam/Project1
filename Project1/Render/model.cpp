#include "../Utility/renafx.h"

#include "../Render/loadObjFile.h"
#include "../Render/textureshader.h"

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
ID3D11ShaderResourceView* MODEL::GetTexture() {	return textureShader->GetTexture(); }

bool MODEL::Initialize(char* modelFilename, WCHAR* textureFilename)
{
	ISFAIL(Load(modelFilename));

	textureShader = new TEXTURESHADER(hwnd, device, deviceContext);
	ISINSTANCE(textureShader);
	if (!textureShader->Initialize(textureFilename))
	{
		ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
	}


	return true;
}


bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos)
{
	file->RenderBuffers();
	ISFAIL(textureShader->Render(file->GetIndexCount(), renderMatrix));

	return true;
}
void MODEL::Shutdown()
{
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
