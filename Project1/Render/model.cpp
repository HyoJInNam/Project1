#include "../Utility/renafx.h"

#include "../Render/loadObjFile.h"
#include "../Render/colorshader.h"
#include "../Render/textureshader.h"

#include "model.h"



MODEL::MODEL()
	: colorShader(nullptr), textureShader(nullptr)
	, local{  D3DXVECTOR3(0.0f, 0.0f, 0.0f)
			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)
			, D3DXVECTOR3(1.0f, 1.0f, 1.0f) }
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

inline void MODEL::TransformScale(float x, float y, float z)
{
	local.scale.x = x;
	local.scale.y = y;
	local.scale.z = z;
}
inline void MODEL::TransformPosition(float x, float y, float z)
{
	local.position.x = x;
	local.position.y = y;
	local.position.z = z;
}
inline void MODEL::TransformRotation(float x, float y, float z)
{
	local.rotation.x = x;
	local.rotation.y = y;
	local.rotation.z = z;
}

inline void MODEL::TransformScale(D3DXVECTOR3 scl)
{
	TransformScale(scl.x, scl.y, scl.z);
}
inline void MODEL::TransformPosition(D3DXVECTOR3 pos)
{
	TransformPosition(pos.x, pos.y, pos.z);
}
inline void MODEL::TransformRotation(D3DXVECTOR3 rot)
{
	TransformRotation(rot.x, rot.y, rot.z);
}

void MODEL::TransformMatrix(TRANSFORM& local)
{
	TransformScale(local.scale);
	TransformPosition(local.position);
	TransformRotation(local.rotation);
}


bool MODEL::Render(PIVOT pivot, RNDMATRIXS& renderMatrix)
{
		{
		D3DXMATRIX S, Rx, Ry, Rz, T;
		D3DXVECTOR3& pos = local.position;
		D3DXVECTOR3& rot = local.rotation;
		D3DXVECTOR3& scl = local.scale;

		D3DXMatrixScaling(&S, scl.x, scl.y, scl.z);
		D3DXMatrixRotationX(&Rx, rot.x);
		D3DXMatrixRotationY(&Ry, rot.y);
		D3DXMatrixRotationZ(&Rz, rot.z);
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
		if (pivot == LOCAL) renderMatrix.world = S * Rx * Ry * Rz * T;
		if (pivot == GLOBAL) renderMatrix.world = S * T * Rx * Ry * Rz;
	}

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


