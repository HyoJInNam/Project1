#include "../Utility/stdafx.h"

#include "../Render/Camera/camera.h"
#include "../Render/Camera/cameraControler.h"

#include "../Render/Lighting/lightclass.h"
#include "../Render/Lighting/lightshader.h"

#include "Panel.h"
#include "../Render/Objects/model.h"
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
	mainCamera = new CameraControl;
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 5.0f, -15.0f);
	mainCamera->SetRotation(18.0f, 0.0f, 0.0f);
	

	light = new LIGHTSHADER; 
	//light->SetDirectionLight();
	light->SetPointLight();
	if (!light->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}

	//=========================================================================

	panel = new PANEL;
	ISINSTANCE(panel);
	if (!panel->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the Panel Object.", L"ERROR");
		return false;
	}

	//=========================================================================

	model = new MODEL;
	ISINSTANCE(model);

	//model->LoadTexture(const_cast<WCHAR*>(L"./data/models/stone01.dds"));
	if (!model->Initialize(const_cast<char*>("./data/models/cube2.txt")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	return 0;
}

void GRAPHICS::Shutdown()
{
	model->Shutdown();

	SAFE_DELETE(panel);
	SAFE_DELETE(light);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	mainCamera->Update();
	panel->Frame();

	return true;
}

BOOL GRAPHICS::Render()
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));


	mainCamera->Render();

	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	mainCamera->GetViewMatrix(matrixs.view);
	transformation->GetProjectionMatrix(matrixs.projection);

	panel->Render(matrixs);

	light->Render(0, matrixs, mainCamera->GetPosition());
	
		model->SetSpin(0, 0.01f, 0);
		model->SetTransformMatrix(matrixs);
		model->Render(matrixs, mainCamera->GetPosition());


	d3d->EndScene();
	return true;
}
