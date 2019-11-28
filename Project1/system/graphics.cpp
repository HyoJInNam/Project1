#include <stdafx.h>
#include "Panel.h"
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
	mainCamera = new CameraControl("main camera");
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 5.0f, -15.0f);
	mainCamera->SetRotation(18.0f, 0.0f, 0.0f);
	


	light = new LIGHT("direction light");
	ISINSTANCE(light);
	light->SetPointLight();
	   
	shader = new LIGHTSHADER; 
	if (!shader->Initialize())
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

	sphere01 = new MODEL("sphere1");
	ISINSTANCE(sphere01);

	if (!sphere01->Initialize(
		const_cast<char*>("./data/models/sphere.txt"),
		const_cast<WCHAR*>(L"./data/models/stone02.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}


	sphere02 = new MODEL("sphere2");
	ISINSTANCE(sphere02);
	if (!sphere02->Initialize(
		const_cast<char*>("./data/models/sphere.txt"),
		const_cast<WCHAR*>(L"./data/models/stone02.dds"),
		const_cast<WCHAR*>(L"./data/models/bump02.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	sphere03 = new MODEL("sphere3");
	ISINSTANCE(sphere03);

	if (!sphere03->Initialize(
		const_cast<char*>("./data/models/sphere.txt"),
		const_cast<WCHAR*>(L"./data/models/stone02.dds"),
		const_cast<WCHAR*>(L"./data/models/bump02.dds"),
		const_cast<WCHAR*>(L"./data/models/spec02.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	sphere01->SetPosition(-4.0f, 0, 0);
	sphere02->SetPosition(0.0f, 0, 0);
	sphere03->SetPosition(4.0f, 0, 0);
	return 0;
}

void GRAPHICS::Shutdown()
{
	sphere03->Shutdown();
	sphere02->Shutdown();
	sphere01->Shutdown();

	SAFE_DELETE(panel);
	SAFE_DELETE(shader);
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
	mainCamera->ViewTransform();
	light->ViewTransform();
	
	panel->Render(matrixs);

	shader->Render(0, matrixs, mainCamera->GetPosition(), nullptr, light->GetLight());
	WindowHierarchy();

	sphere01->SetSpin(0.0f, 0.01f, 0.0f);
	sphere01->Render(matrixs, mainCamera->GetPosition(), light);

	sphere02->SetSpin(0.0f, 0.01f, 0.0f);
	sphere02->Render(matrixs, mainCamera->GetPosition(), light);

	sphere03->SetSpin(0.0f, 0.01f, 0.0f);
	sphere03->Render(matrixs, mainCamera->GetPosition(), light);

	//WINDX_IMGUI::GetInstance()->Render();
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	d3d->EndScene();
	return true;
}


void GRAPHICS::WindowHierarchy()
{
	ImGui::Begin("Hierarchy");

	mainCamera->ShowObjectInspector();
	//panel->ShowWindowHierarchy();
	light->ShowObjectInspector();
	sphere01->ShowObjectInspector();
	sphere02->ShowObjectInspector();
	sphere03->ShowObjectInspector();
	
	ImGui::End();
}