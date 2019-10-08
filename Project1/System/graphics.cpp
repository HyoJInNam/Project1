#include "../stdafx.h"

#include "../Render/camera.h"
#include "../Render/model.h"
#include "../Render/colorshader.h"
//==============================


#include "graphics.h"


GRAPHICS::GRAPHICS()
{
	D3D::GetInstance()->Initialize();
}
GRAPHICS::~GRAPHICS() {}
BOOL GRAPHICS::Initialize()
{
	//cameraRender = new R_TRANSFORMATION;
	//ISINSTANCE(cameraRender);
	//cameraRender->Initialize(); 

	mainCamera = new CAMERA;
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 0.0f, -5.0f);


	cube = new MODEL;
	ISINSTANCE(cube);
	if (!cube->Initialize(const_cast<char*>("./Engine/data/cube.txt")))
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
	//SAFE_DELETE(cameraRender);
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
	ISFAIL(result);
	return true;
}

BOOL GRAPHICS::Render(float rotation)
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));
	
	mainCamera->Render();

	RNDMatrix cameraMatrix;
	//cameraRender->GetWorldMatrix(cameraMatrix.world);
	//mainCamera->GetViewMatrix(cameraMatrix.view);
	//cameraRender->GetProjectionMatrix(cameraMatrix.projection);

	mainCamera->GetWorldMatrix(cameraMatrix.world);
	mainCamera->GetViewMatrix(cameraMatrix.view);
	mainCamera->GetProjectionMatrix(cameraMatrix.projection);

	//D3DXMatrixRotationY(&cameraMatrix.world, rotation);

	cube->Render();

	if (!colorShader->Render(cube->GetIndexCount(), cameraMatrix))
	{
		return false;
	}

	d3d->EndScene();
	return true;
}
