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
		const_cast<char*>("./data/models/basic/plane01.txt"),
		const_cast<WCHAR*>(L"./data/models/basic/gray.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the ground model object.", L"ERROR");
		return false;
	}

	m_GroundModel->SetPosition(0.0f, -1.0f, 0.0f);
	m_GroundModel->SetScale(10.0f, 10.0f, 10.0f);



	//=========================================================================

	float modelSize = 0.05f;
	char* modelName = const_cast<char*>("./data/models/robot/robot.txt");
	WCHAR* modelColor = const_cast<WCHAR*>(L"./data/models/robot/robot_c.png");
	WCHAR* modelNormal = const_cast<WCHAR*>(L"./data/models/robot/robot_n.png");
	WCHAR* modelSpec = const_cast<WCHAR*>(L"./data/models/robot/robot_s.png");
	//float modelSize = 1.0f;
	//char* modelName = const_cast<char*>("./data/models/basic/teapot.obj");
	//WCHAR* modelColor = const_cast<WCHAR*>(L"./data/models/maps_c/pink.png");
	//WCHAR* modelNormal = const_cast<WCHAR*>(L"./data/models/maps_n/n07.jpg");
	//WCHAR* modelNormal2 = const_cast<WCHAR*>(L"./data/models/maps_n/n06.jpg");
	//WCHAR* modelSpec = const_cast<WCHAR*>(L"./data/models/maps_s/spec02.png");

	model01 = new MODEL("teapot");
	ISINSTANCE(model01);
	if (!model01->Initialize(modelName, modelColor, modelNormal, modelSpec))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}


	model01->SetScale(modelSize, modelSize, modelSize);
	model01->SetPosition(0.0f, -2.0f, -3.0f);
	return 0;
}

void GRAPHICS::Shutdown()
{
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

	model01->SetSpin(0.0f, 0.01f, 0.0f);
	model01->Render(matrixs, mainCamera->GetPosition(), light);
	//model01->RenderShadow(matrixs, d3d->GetShaderResourceView(), light);


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
	//m_GroundModel->ShowObjectInspector();
	model01->ShowObjectInspector();

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
	ISFAIL(m_GroundModel->RenderDepth(matrixs));

	
	d3d->SetBackBufferRenderTarget();
	transformation->ResetViewport(d3d->GetDeviceContext());

	return true;
}
