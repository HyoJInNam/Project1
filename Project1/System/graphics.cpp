#include "../stdafx.h"
#include "../Render/camera.h"
#include "../Render/model.h"
#include "../Render/colorshaderclass.h"
//==============================


#include "graphics.h"


GRAPHICS::GRAPHICS()
{
	D3D::GetInstance()->Initialize();
}
GRAPHICS::~GRAPHICS() {}
BOOL GRAPHICS::Initialize()
{
	mainCamera = new CAMERA;
	ERR_INSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 0.0f, -5.0f);


	cube = new MODEL;
	ERR_INSTANCE(cube);
	if (!cube->Initialize(D3D::GetInstance()->GetDevice(), const_cast<char*>("./Engine/data/cube.txt")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.");
		return false;
	}

	// Create the color shader object.
	colorShader = new COLORSHADER;
	ERR_INSTANCE(colorShader);
	if (!colorShader->Initialize(D3D::GetInstance()->GetDevice(), WNDDesc::GetInstance()->getHwnd()))
	{
		ERR_MESSAGE(L"Could not initialize the color shader object.");
		return false;
	}


	return 0;
}

void GRAPHICS::Shutdown()
{
	SAFE_DELETE(cube);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(colorShader);
}


BOOL GRAPHICS::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

BOOL GRAPHICS::Render(float rotation)
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	mainCamera->Render();
	D3DXMATRIX world, view, projection;
	d3d->GetWorldMatrix(world);
	mainCamera->GetViewMatrix(view);
	d3d->GetProjectionMatrix(projection);

	D3DXMatrixRotationY(&world, rotation);

	cube->Render(d3d->GetDeviceContext());

	if (!colorShader->Render(d3d->GetDeviceContext(), cube->GetIndexCount(), world, view, projection))
	{
		return false;
	}

	d3d->EndScene();
	return true;
}
