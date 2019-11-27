#include <stdafx.h>
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
	if (!bitmap->Initialize(const_cast<WCHAR*>(L"./data/models/gray.jpg")))
	{
		ERR_MESSAGE(L"Could not initialize the light bitmap model.", L"ERROR");
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

	//FULL SIZE
	//bitmap->Resize(1370 * 1.7, 750 * 1.7);
	bitmap->Render(PanelMatrixs);

	d3d->TurnOffAlphaBlending();
	d3d->TurnZBufferOn();

	return true;
}

