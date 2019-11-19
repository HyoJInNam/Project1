#include "../Utility/stdafx.h"

#include "../Render/Camera/camera.h"
#include "../Render/Camera/cameraPanelControler.h"

#include "../Render/Objects/BitmapClass.h"
//================================
#include "Panel.h"

PANEL::PANEL()
{
	D3D::GetInstance()->Initialize();

	transformation = new RENDER_T;
	transformation->Initialize();
}

PANEL::~PANEL() {}


BOOL PANEL::Initialize()
{
	PanelCamera = new CameraPanelControler;
	ISINSTANCE(PanelCamera);
	PanelCamera->SetPosition(0.0f, 0.0f, -15.0f);

	//=============================================

	bitmap = new BitmapClass;
	ISINSTANCE(bitmap);
	if (!bitmap->Initialize(const_cast<WCHAR*>(L"./data/models/seafloor.dds"), 256, 256))
	{
		ERR_MESSAGE(L"Could not initialize the light bitmap object.", L"ERROR");
		return false;
	}
	return true ;
}
void PANEL::Shutdown()
{
	bitmap->Shutdown();

	SAFE_DELETE(PanelCamera);
	SAFE_DELETE(transformation);
}

BOOL PANEL::Frame()
{
	PanelCamera->Update();

	return true;
}

BOOL PANEL::Render(RNDMATRIXS& matrixs)
{

	D3D* d3d = D3D::GetInstance();
	

	d3d->TurnZBufferOff();
	d3d->TurnOnAlphaBlending();


	PanelCamera->Render();

	RNDMATRIXS PanelMatrixs;
	transformation->GetWorldMatrix(PanelMatrixs.world);
	PanelCamera->GetViewMatrix(PanelMatrixs.view);
	transformation->GetProjectionMatrix(PanelMatrixs.projection);

	bitmap->Render(PanelMatrixs, 1, 1);

	d3d->TurnOffAlphaBlending();
	d3d->TurnZBufferOn();

	return true;
}

