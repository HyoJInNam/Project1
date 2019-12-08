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
	


	light = new LIGHT("main light");
	ISINSTANCE(light);
	if (!light->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}
	light->SetDirectionLight();
	light->SetPosition(0.1f, 0.8f, 0.8f);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateOrthoMatrix(SCREEN_DEPTH, SCREEN_NEAR);




	//=========================================================================
	/*
	panel = new PANEL;
	ISINSTANCE(panel);
	if (!panel->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the Panel Object.", L"ERROR");
		return false;
	}*/

	skydome = new DOME("sky dome");
	ISINSTANCE(skydome);

	if (!skydome->Initialize(
		const_cast<char*>("./data/system/skydome.txt")))
	{
		ERR_MESSAGE(L"Could not initialize the sky dome model object.", L"ERROR");
		return false;
	}

	m_GroundModel = new MODEL("back ground");
	ISINSTANCE(m_GroundModel);

	if (!m_GroundModel->Initialize(
		const_cast<char*>("./data/models/plane01.txt"),
		const_cast<WCHAR*>(L"./data/models/gray.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the ground model object.", L"ERROR");
		return false;
	}

	m_GroundModel->SetPosition(0.0f, -1.0f, 0.0f);
	m_GroundModel->SetScale(10.0f, 10.0f, 10.0f);



	//=========================================================================

	model01 = new MODEL("left model : toon shader");
	ISINSTANCE(model01);
	model01->IsInk = true;

	if (!model01->Initialize(
		const_cast<char*>("./data/models/model/model2.txt"),
		const_cast<WCHAR*>(L"./data/models/model/body.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}


	model02 = new MODEL("center model : ink shader");
	ISINSTANCE(model02);
	if (!model02->Initialize(
		const_cast<char*>("./data/models/model/model2.txt"),
		const_cast<WCHAR*>(L"./data/models/model/body.png"), 
		const_cast<WCHAR*>(L"./data/models/model/bodynomal.png"),
		const_cast<WCHAR*>(L"./data/models/model/body_ao_bake.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	model03 = new MODEL("right model : bump shader");
	ISINSTANCE(model03);
	if (!model03->Initialize(
		const_cast<char*>("./data/models/model/model2.txt"),
		const_cast<WCHAR*>(L"./data/models/model/body.png"),
		const_cast<WCHAR*>(L"./data/models/model/bodynomal.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	model01->SetScale(0.001f, 0.001f, 0.001f);
	model02->SetScale(0.001f, 0.001f, 0.001f);
	model03->SetScale(0.001f, 0.001f, 0.001f);

	model01->SetPosition(-5.0f, 0, 0);
	model02->SetPosition(0.0f, 0, 0);
	model03->SetPosition(5.0f, 0, 0);
	return 0;
}

void GRAPHICS::Shutdown()
{
	model03->Shutdown();
	model02->Shutdown();
	model01->Shutdown();

	m_GroundModel->Shutdown();

	//SAFE_DELETE(panel);
	light->Shutdown();
	SAFE_DELETE(light);
	SAFE_DELETE(skydome);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	mainCamera->Update();
	//panel->Frame();

	return true;
}

BOOL GRAPHICS::Render()
{
	D3D* d3d = D3D::GetInstance();

	//ISFAIL(RenderSceneToTexture());
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));
	
	mainCamera->Render();
	light->GenerateViewMatrix();

	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	mainCamera->GetViewMatrix(matrixs.view);
	transformation->GetProjectionMatrix(matrixs.projection);
	mainCamera->ViewTransform();

	light->GetViewMatrix(matrixs.lightView);
	light->GetOrthoMatrix(matrixs.lightProjection);
	light->ViewTransform();

	//panel->Render(matrixs);
	D3DXMatrixTranslation(&matrixs.world, mainCamera->GetPosition().x, mainCamera->GetPosition().y, mainCamera->GetPosition().z);
	skydome->Render(d3d, matrixs);
	transformation->GetWorldMatrix(matrixs.world);

	light->Render(0, matrixs, mainCamera->GetPosition(), nullptr);
	WindowHierarchy();

	//sphere01->SetSpin(0.0f, 0.01f, 0.0f);
	//sphere02->SetSpin(0.0f, 0.01f, 0.0f);
	//sphere03->SetSpin(0.0f, 0.01f, 0.0f);

	model01->Render(matrixs, mainCamera->GetPosition(), light);
	model02->Render(matrixs, mainCamera->GetPosition(), light);
	model03->Render(matrixs, mainCamera->GetPosition(), light);

	//sphere01->RenderShadow(matrixs, d3d->GetShaderResourceView(), light);
	//sphere02->RenderShadow(matrixs, d3d->GetShaderResourceView(), light);
	//sphere03->RenderShadow(matrixs, d3d->GetShaderResourceView(), light);


	//m_GroundModel->Render(matrixs, mainCamera->GetPosition(), light);
	//m_GroundModel->RenderShadow(matrixs, d3d->GetShaderResourceView(), light);


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
	light->ShowObjectInspector();
	//skydome->ShowObjectInspector();
	//panel->ShowWindowHierarchy();
	m_GroundModel->ShowObjectInspector();
	model01->ShowObjectInspector();
	model02->ShowObjectInspector();
	model03->ShowObjectInspector();
	
	ImGui::End();
}

bool GRAPHICS::RenderSceneToTexture()
{
	D3D* d3d = D3D::GetInstance();

	d3d->SetRenderTarget();
	transformation->ResetViewport(d3d->GetDeviceContext());
	d3d->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	light->GenerateViewMatrix();
	light->GetViewMatrix(matrixs.lightView);
	light->GetOrthoMatrix(matrixs.lightProjection);


	ISFAIL(model01->RenderDepth(matrixs));
	ISFAIL(model02->RenderDepth(matrixs));
	ISFAIL(model03->RenderDepth(matrixs));
	ISFAIL(m_GroundModel->RenderDepth(matrixs));

	
	d3d->SetBackBufferRenderTarget();
	transformation->ResetViewport(d3d->GetDeviceContext());

	return true;
}
