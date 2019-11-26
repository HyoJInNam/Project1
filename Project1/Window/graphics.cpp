#include <stdafx.h>
#include "Panel.h"
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
	mainCamera = new CameraControl;
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 5.0f, -15.0f);
	mainCamera->SetRotation(18.0f, 0.0f, 0.0f);
	


	light = new LIGHT;
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
	sphere = new MODEL;
	ISINSTANCE(sphere);
	if (!sphere->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/stone01.dds")
		, const_cast<WCHAR*>(L"./data/models/bump01.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	model = new MODEL;
	ISINSTANCE(model);

	if (!model->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/stone02.dds")
		, const_cast<WCHAR*>(L"./data/models/bump02.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	sphere2 = new MODEL;
	ISINSTANCE(sphere2);

	if (!sphere2->Initialize(const_cast<char*>("./data/models/sphere.txt")
							, const_cast<WCHAR*>(L"./data/models/earthtexture.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	sphere->SetTransformPosition(-2.0f, 0, 0);
	model->SetTransformPosition(2.0f, 0, 0);
	sphere2->SetTransformPosition(6.0f, 0, 0);
	return 0;
}

void GRAPHICS::Shutdown()
{
	sphere2->Shutdown();
	model->Shutdown();
	sphere->Shutdown();

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
	
	panel->Render(matrixs);

	shader->Render(0, matrixs, mainCamera->GetPosition(), light->GetLight());

	model->SetSpin(0, 0.01f, 0);
	model->SetTransformMatrix(matrixs);
	model->Render(matrixs, mainCamera->GetPosition());

	sphere->SetSpin(0, 0.01f, 0);
	sphere->SetTransformMatrix(matrixs);
	sphere->Render(matrixs, mainCamera->GetPosition());

	sphere2->SetSpin(0, 0.01f, 0);
	sphere2->SetTransformMatrix(matrixs);
	sphere2->Render(matrixs, mainCamera->GetPosition());
	ViewTransform(sphere2);

	//WINDX_IMGUI::GetInstance()->Render();
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	d3d->EndScene();
	return true;
}


void GRAPHICS::ViewTransform(MODEL* object)
{
	ImGui::Begin("object");
	{
		ImGui::Text("transform");
		D3DXVECTOR3 pos = object->GetPosition();
		ImGui::DragFloat3("position", (float*)&pos, 0.1f, 0, 0);
		object->SetTransformPosition(pos);

		D3DXVECTOR3 rot = object->GetRotation();
		ImGui::DragFloat3("rotation", (float*)&rot, 0.1f, 0, 0);
		object->SetTransformRotation(rot);

		D3DXVECTOR3 scl = object->GetScale();
		ImGui::DragFloat3("scale", (float*)&scl, 0.1f, 0, 0);
		object->SetTransformScale(scl);


		ImGui::Text("light");
		LIGHT* obj_light = object->GetLIGHT();
		D3DXVECTOR4 ambient = obj_light->GetAmbientColor();
		ImGui::DragFloat4("ambient", (float*)&ambient, 0.1f, 0, 0);
		obj_light->SetAmbientColor(ambient);

		D3DXVECTOR4 diffuse = obj_light->GetDiffuseColor();
		ImGui::DragFloat4("diffuse", (float*)&diffuse, 0.1f, 0, 0);
		obj_light->SetDiffuseColor(diffuse);

		D3DXVECTOR3 direction = obj_light->GetDirection();
		ImGui::DragFloat3("direction", (float*)&direction, 0.1f, 0, 0);
		obj_light->SetDirection(direction);

		D3DXVECTOR4 specular = obj_light->GetSpecularColor();
		ImGui::DragFloat4("specular", (float*)&specular, 0.1f, 0, 0);
		obj_light->SetSpecularColor(specular);

		float specularPower = obj_light->GetSpecularPower();
		ImGui::DragFloat("specular power", (float*)&specularPower, 0.1f, 0, 0);
		obj_light->SetSpecularPower(specularPower);
		object->SetLIGHT(obj_light);
	}
	ImGui::End();
}