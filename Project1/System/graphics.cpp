#include "../Utility/stdafx.h"
#include "../Render/camera.h"
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
	mainCamera = new CAMERA;
	ISINSTANCE(mainCamera);
	mainCamera->SetPosition(0.0f, 0.0f, -10.0f);



	Solar = new MODEL;
	ISINSTANCE(Solar);
	if (!Solar->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/solartexture.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	Mercury = new MODEL;
	ISINSTANCE(Mercury);
	if (!Mercury->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/01_pink.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}
	Venus = new MODEL;
	ISINSTANCE(Venus);
	if (!Venus->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/02_yellow.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	Earth = new MODEL;
	ISINSTANCE(Earth);
	if (!Earth->Initialize(const_cast<char*>("./data/models/sphere.txt")
		, const_cast<WCHAR*>(L"./data/models/03_blue.png")))
	{
		ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
		return false;
	}

	//moon = new MODEL;
	//ISINSTANCE(moon);
	//if (!moon->Initialize(const_cast<char*>("./data/models/sphere.txt")
	//	, const_cast<WCHAR*>(L"./data/models/moontexture.png")))
	//{
	//	ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
	//	return false;
	//}

	for (int i = 0; i < 3; i++)
	{
		sphere[3 * i + 0] = new MODEL;
		ISINSTANCE(sphere[3 * i + 0]);
		if (!sphere[3 * i + 0]->Initialize(const_cast<char*>("./data/models/sphere.txt")
			, const_cast<WCHAR*>(L"./data/models/01_pink.png")))
		{
			ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
			return false;
		}


		sphere[3 * i + 1] = new MODEL;
		ISINSTANCE(sphere[3 * i + 1]);
		if (!sphere[3 * i + 1]->Initialize(const_cast<char*>("./data/models/sphere.txt")
			, const_cast<WCHAR*>(L"./data/models/02_yellow.png")))
		{
			ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
			return false;
		}

		sphere[3 * i + 2] = new MODEL;
		ISINSTANCE(sphere[3 * i + 2]);
		if (!sphere[3 * i + 2]->Initialize(const_cast<char*>("./data/models/sphere.txt")
			, const_cast<WCHAR*>(L"./data/models/03_blue.png")))
		{
			ERR_MESSAGE(L"Could not initialize the model object.", L"ERROR");
			return false;
		}


		sphere[3 * i + 0]->SetTransformScale(Solar->GetScale() / 20);
		sphere[3 * i + 1]->SetTransformScale(Solar->GetScale() / 20);
		sphere[3 * i + 2]->SetTransformScale(Solar->GetScale() / 20);
	}


	Mercury->SetTransformScale(Solar->GetScale() / 6);
	//Mercury->SetTransformRotation(0.3f, 0.0f, 0.0f);
	//Mercury->SetTransformPosition(2.0f, 0, 0);
	Mercury->SetTransformPosition(3.0f, 0, 0);

	Venus->SetTransformScale(Solar->GetScale() / 5);
	//Venus->SetTransformRotation(0.3f, 0.0f, 0.0f);
	//Venus->SetTransformPosition(2.7f, 0, 0);
	Venus->SetTransformPosition( 0, 3.0f, 0);

	Earth->SetTransformScale(Solar->GetScale() / 4);
	//Earth->SetTransformRotation(0.3f, 0.0f, 0.0f);
	//Earth->SetTransformPosition(3.8f, 0, 0);
	Earth->SetTransformPosition(0, -3.0f, 0);

	//moon->SetTransformScale(Earth->GetScale() / 3);
	////moon->SetTransformRotation(0.0f, 0.0f, 0.0f);
	//moon->SetTransformPosition(4.3f, 0, 0);

	for (int i = 0; i < 3; i++)
	{
		sphere[0 + i]->SetParent(Mercury);
		sphere[3 + i]->SetParent(Venus);
		sphere[6 + i]->SetParent(Earth);
	}
	for (int i = 0; i < 3; i++)
	{
		sphere[3 * i + 0]->SetTransformPosition(sphere[3 * i + 0]->GetParent()->GetPosition() + D3DXVECTOR3(0.5, 0, 0));
		sphere[3 * i + 1]->SetTransformPosition(sphere[3 * i + 1]->GetParent()->GetPosition() + D3DXVECTOR3(0, 0.5, 0));
		sphere[3 * i + 2]->SetTransformPosition(sphere[3 * i + 2]->GetParent()->GetPosition() + D3DXVECTOR3(0, -0.5, 0));
	}
	return 0;
}

void GRAPHICS::Shutdown()
{
	for (int i = 0; i < 9; i++)
		sphere[i]->Shutdown();
	
	//moon->Shutdown();
	Earth->Shutdown();
	Venus->Shutdown();
	Mercury->Shutdown();
	Solar->Shutdown();
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(transformation);
}


BOOL GRAPHICS::Frame()
{
	D3D* d3d = D3D::GetInstance();
	d3d->BeginScene(D3DXCOLOR(0, 0, 0, 1.0f));

	mainCamera->Render();
	RNDMATRIXS matrixs;
	transformation->GetWorldMatrix(matrixs.world);
	mainCamera->GetViewMatrix(matrixs.view);
	transformation->GetProjectionMatrix(matrixs.projection);
	
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	ISFAIL(Render(matrixs, rotation));

	d3d->EndScene();
	return true;
}

BOOL GRAPHICS::Render(RNDMATRIXS& matrixs, float rotation)
{
	//Time.deltaTime 구현 할것.
	float speed = 0.05f; 
	float parentOrbitSpeed = 0.005f;
	float orbitSpeed = 50.0;// 0.04f;

	//Solar->SetTransformPosition(0, sin(rotation), 0);
	Solar->SetTransformPosition(sin(rotation)*speed, 0, 0);
	Solar->SetSpin(0, speed, 0);
	Solar->SetTransformMatrix(matrixs);
	Solar->Render(matrixs, mainCamera->GetPosition());

	Mercury->SetParent(Solar);
	//Mercury->SetSpin(0, speed, 0);
	Mercury->SetTransformMatrix(matrixs);
	//Mercury->SetOrbitRot(0, orbitSpeed, 0);
	Mercury->SetOrbitRot(0, parentOrbitSpeed, 0);
	Mercury->Render(matrixs, mainCamera->GetPosition());

	Venus->SetParent(Solar);
	//Venus->SetSpin(0, 0, speed);
	Venus->SetTransformMatrix(matrixs);
	//Venus->SetOrbitRot(0, orbitSpeed, 0);
	Venus->SetOrbitRot(parentOrbitSpeed,0,  0);
	Venus->Render(matrixs, mainCamera->GetPosition());

	Earth->SetParent(Solar);
	//Earth->SetSpin(speed, 0, 0);
	Earth->SetTransformMatrix(matrixs);
	//Earth->SetOrbitRot(0, orbitSpeed, 0);
	Earth->SetOrbitRot(0, 0, parentOrbitSpeed);
	Earth->Render(matrixs, mainCamera->GetPosition());

	//moon->SetParent(Earth);
	//moon->SetSpin(0, speed, 0);
	//moon->SetTransformMatrix(matrixs);
	//moon->SetOrbitRot(0, orbitSpeed, 0);
	//moon->Render(matrixs, mainCamera->GetPosition());

	for (int i = 0; i < 3; i++)
	{
		sphere[3 * i + 0]->SetOrbitRot(0, orbitSpeed, 0);
		sphere[3 * i + 1]->SetOrbitRot(orbitSpeed, 0, 0);
		sphere[3 * i + 2]->SetOrbitRot(0, 0, orbitSpeed);
	}
	for (int i = 0; i < MODELCOUNT; i++)
	{
		sphere[i]->SetTransformMatrix(matrixs);
		sphere[i]->Render(matrixs, mainCamera->GetPosition());
	}

	return true;
}
