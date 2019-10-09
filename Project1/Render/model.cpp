#include "renafx.h"
#include "model.h"



MODEL::MODEL()
	: colorShader(nullptr), textureShader(nullptr)
	, transform{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) , D3DXVECTOR3(0.0f, 0.0f, 0.0f) }
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

	textureShader = new TEXTURESHADER(hwnd, device, deviceContext);
	ISINSTANCE(textureShader);
	if (!textureShader->Initialize(textureFilename))
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
	
	ISFAIL(textureShader->Render(file->GetIndexCount(), renderMatrix));
	return true;
}
void MODEL::Shutdown()
{
	SAFE_DELETE(textureShader);
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


//=====================================================

bool MODEL::IsRotation(D3DXMATRIX& matrix, RotationDirection dir, float rotation)
{
	switch (dir) {
	case X:
		D3DXMatrixRotationX(&matrix, rotation);
		return true;
	case Y:
		D3DXMatrixRotationY(&matrix, rotation);
		return true;
	case Z:
		D3DXMatrixRotationZ(&matrix, rotation);
		return true;
	}

	return false;
}

bool MODEL::IsOrbit(D3DXMATRIX& matrix, D3DXVECTOR3 dis, float speed)
{
	D3DXMATRIX R, T;

	D3DXMatrixTranslation(&T, dis.x, dis.y, dis.z);
	IsRotation(matrix, Y, speed);

	matrix = T * R;

	return true;
}
