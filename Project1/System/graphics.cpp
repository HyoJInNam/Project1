#include "../stdafx.h"
#include "../Render/camera.h"
#include "../Render/model.h"
#include "../Render/colorshader.h"
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
	mainCamera->SetPosition(0.0f, 0.0f, -5.0f);


	cube = new MODEL;
	ISINSTANCE(cube);
	if (!cube->Initialize(const_cast<char*>("./data/models/sphere.txt")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	colorShader = new COLORSHADER;
	ISINSTANCE(colorShader);
	if (!colorShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the color shader object.", L"ERROR");
		return false;
	}


	return 0;
}

void GRAPHICS::Shutdown()
{
	SAFE_DELETE(colorShader);
	SAFE_DELETE(cube);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	ISFAIL(Render(rotation));
	return true;
}

BOOL GRAPHICS::Render(float rotation)
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	mainCamera->Render();
	RNDMATRIXS render;
	transformation->GetWorldMatrix(render.world);
	mainCamera->GetViewMatrix(render.view);
	transformation->GetProjectionMatrix(render.projection);

	D3DXMatrixRotationY(&render.world, rotation);

	cube->Render();

	if (!colorShader->Render(cube->GetIndexCount(), render))
	{
		return false;
	}

	d3d->EndScene();
	return true;
}
