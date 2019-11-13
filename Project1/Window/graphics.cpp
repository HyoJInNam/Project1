#include "../Utility/stdafx.h"

#include "../Text/TextClass.h"
#include "../Render/camera.h"
#include "../Render/model.h"
#include "../Lighting/lightclass.h"
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


	//=========================================================================


	Solar = new MODEL;
	ISINSTANCE(Solar);
	if (!Solar->Initialize(const_cast<char*>("./data/models/sphere.txt")
						 , const_cast<WCHAR*>(L"./data/models/solartexture.jpg")
						 , LIGHT_DIRECTION))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	Mercury = new MODEL;
	ISINSTANCE(Mercury);
	if (!Mercury->Initialize(const_cast<char*>("./data/models/sphere.txt")
						   , const_cast<WCHAR*>(L"./data/models/mercuryTexture.jpg")
						   , LIGHT_POINTLIGHT))
	{				
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}
	Venus = new MODEL;
	ISINSTANCE(Venus);
	if (!Venus->Initialize(const_cast<char*>("./data/models/sphere.txt")
						 , const_cast<WCHAR*>(L"./data/models/venusTexture.jpg")
						 , LIGHT_POINTLIGHT))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	Earth = new MODEL;
	ISINSTANCE(Earth);
	if (!Earth->Initialize(const_cast<char*>("./data/models/sphere.txt")
						 , const_cast<WCHAR*>(L"./data/models/earthtexture.jpg")
						 , LIGHT_POINTLIGHT))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	moon = new MODEL;
	ISINSTANCE(moon);
	if (!moon->Initialize(const_cast<char*>("./data/models/sphere.txt")
						, const_cast<WCHAR*>(L"./data/models/moontexture.png")
						, LIGHT_POINTLIGHT))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	{
		Mercury->SetParent(Solar);
		Venus->SetParent(Solar);
		Earth->SetParent(Solar);
		moon->SetParent(Earth);


		Mercury->SetTransformScale(Solar->GetScale() / 6);
		Mercury->SetTransformPosition(2.0f, 0, 0);
		Mercury->GetLight()->SetSpecularPower(4.0f);

		Venus->SetTransformScale(Solar->GetScale() / 5);
		Venus->SetTransformPosition(2.7f, 0.2f, 0);
		Venus->GetLight()->SetSpecularPower(10.0f);

		Earth->SetTransformScale(Solar->GetScale() / 4);
		Earth->SetTransformRotation(0.3f, 0.0f, 0.0f);
		Earth->SetTransformPosition(3.8f, 0.1f, 0);
		Earth->GetLight()->SetSpecularPower(50.0f);

		moon->SetTransformScale(Earth->GetScale() / 3);
		moon->SetTransformPosition(moon->GetParent()->GetPosition() + D3DXVECTOR3(0.5f, 0.1f, 0));
	}
	return 0;
}

void GRAPHICS::Shutdown()
{
	moon->Shutdown();
	Earth->Shutdown();
	Venus->Shutdown();
	Mercury->Shutdown();
	Solar->Shutdown();

	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	//=============================================================
	   
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	return true;
}

BOOL GRAPHICS::Render()
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	//=============================================================

	mainCamera->Render();
	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	mainCamera->GetViewMatrix(matrixs.view);
	transformation->GetProjectionMatrix(matrixs.projection);

	{
		//Time.deltaTime ���� �Ұ�.
		float speed = 0.05f;
		float parentOrbitSpeed = 0.005f;
		float orbitSpeed = 0.05f;

		Solar->SetSpin(0, speed, 0);
		Solar->SetTransformMatrix(matrixs);
		Solar->Render(matrixs, mainCamera->GetPosition());

		Mercury->SetSpin(0, speed, 0);
		Mercury->SetTransformMatrix(matrixs);
		Mercury->SetOrbitRot(parentOrbitSpeed, parentOrbitSpeed, 0);
		Mercury->Render(matrixs, mainCamera->GetPosition());

		Venus->SetSpin(0, speed, 0);
		Venus->SetTransformMatrix(matrixs);
		Venus->SetOrbitRot(parentOrbitSpeed, parentOrbitSpeed, 0);
		Venus->Render(matrixs, mainCamera->GetPosition());

		Earth->SetSpin(0, speed, 0);
		Earth->SetTransformMatrix(matrixs);
		Earth->SetOrbitRot(parentOrbitSpeed, parentOrbitSpeed, 0);
		Earth->Render(matrixs, mainCamera->GetPosition());

		moon->SetSpin(0, speed, 0);
		moon->SetTransformMatrix(matrixs);
		moon->SetOrbitRot(orbitSpeed, orbitSpeed, 0);
		moon->Render(matrixs, mainCamera->GetPosition());
	}
	//===========================================================
	
	d3d->TurnZBufferOff();
	d3d->TurnOnAlphaBlending();

	//ISFAIL(m_Text->Render(textCameraRenderMatrix.world, textCameraRenderMatrix.ortho));

	d3d->TurnOffAlphaBlending();
	d3d->TurnZBufferOn();


	//===========================================================

	d3d->EndScene();
	return true;
}
