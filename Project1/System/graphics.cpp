#include "../stdafx.h"

#include "render/camera.h"
#include "render/model.h"
#include "render/colorshaderclass.h"

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
	if (!cube->Initialize(D3D::GetInstance()->GetDevice()))
	{
		ERR_MESSAGE(L"Could not initialize the model object.");
		return false;
	}

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
	SAFE_DELETE(colorShader);
	SAFE_DELETE(cube);
	SAFE_DELETE(mainCamera);
}


BOOL GRAPHICS::Frame()
{
	return Render();
}

BOOL GRAPHICS::Render()
{
	D3D::GetInstance()->BeginScene(D3DXCOLOR(225.0f, 225.0f, 225.0f, 1.0f));

	mainCamera->Render();
	D3DXMATRIX world, view, projection;
	D3D::GetInstance()->GetWorldMatrix(world);
	mainCamera->GetViewMatrix(view);
	D3D::GetInstance()->GetProjectionMatrix(projection);

	cube->Render(D3D::GetInstance()->GetDeviceContext());
	
	if (!colorShader->Render(D3D::GetInstance()->GetDeviceContext(),
		cube->GetIndexCount(), world, view, projection))
	{
		return false;
	}

	D3D::GetInstance()->EndScene();
	return true;
}
