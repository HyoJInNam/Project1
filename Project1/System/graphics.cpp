#include "../stdafx.h"
#include "../Render/camera.h"
#include "../Render/model.h"
//==============================

#include "graphics.h"


GRAPHICS::GRAPHICS()
{
	D3D::GetInstance()->Initialize();

	transformation = new RENDER_T;
	transformation->Initialize();
}
GRAPHICS::~GRAPHICS() {}
BOOL GRAPHICS::Initialize()
{
	mainCamera = new CAMERA;
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 0.0f, -10.0f);


	sphere = new MODEL;
	ISINSTANCE(sphere);
	if (!sphere->Initialize(const_cast<char*>("./data/models/sphere.txt")
							, const_cast<WCHAR*>(L"./data/models/earthtexture.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	moon = new MODEL;
	ISINSTANCE(moon);
	if (!moon->Initialize(const_cast<char*>("./data/models/sphere.txt")
						, const_cast<WCHAR*>(L"./data/models/moontexture.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	return 0;
}

void GRAPHICS::Shutdown()
{
	SAFE_DELETE_ARRAY(moon);
	SAFE_DELETE_ARRAY(sphere);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	mainCamera->Render();
	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	mainCamera->GetViewMatrix(matrixs.view);
	transformation->GetProjectionMatrix(matrixs.projection);
	
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	ISFAIL(Render(matrixs, rotation));

	d3d->EndScene();
	return true;
}

BOOL GRAPHICS::Render(RNDMATRIXS& matrixs, float rotation)
{
	D3DXMATRIX S, Rx, Ry, T;

	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);
	sphere->IsRotation(Ry, Y, rotation);
	D3DXMatrixTranslation(&T, 0.0f, 0.0f, 0.0f);
	matrixs.world = S * Ry * T;
	sphere->Render(matrixs);


	D3DXMatrixScaling(&S, 0.3f, 0.3f, 0.3f);
	D3DXMatrixRotationX(&Rx, rotation);
	D3DXMatrixRotationY(&Ry, rotation);
	D3DXMatrixTranslation(&T, 3.0f, 1.0f, 0.0f);
	matrixs.world = S * T * Rx * Ry;

	moon->Render(matrixs);
	return true;
}
