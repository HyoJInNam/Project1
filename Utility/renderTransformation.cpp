
#include "Utility.h"
#include "renderTransformation.h"


RENDERTRANSFORMATION::RENDERTRANSFORMATION()
{
	WNDDesc* wnd = WNDDesc::GetInstance();

	UINT width, height;
	wnd->getSceneSize(width, height);
	screenWidth = (FLOAT)width;
	screenHeight = (FLOAT)height;

	wnd->getSceneDepth(screenNear, screenDepth);
}

RENDERTRANSFORMATION::~RENDERTRANSFORMATION() {}


void RENDERTRANSFORMATION::Initialize()
{
	CreateViewport();
}

void RENDERTRANSFORMATION::CreateViewport()
{
	{
		viewport.Width = screenWidth;
		viewport.Height = screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		D3D::GetInstance()->GetDeviceContext()->RSSetViewports(1, &viewport);
	}


	float fieldOfView, screenAspect;

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = screenWidth / screenHeight;


	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&matrix.projection, fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&matrix.world);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&matrix.ortho, screenWidth, screenHeight, screenNear, screenDepth);

}


void RENDERTRANSFORMATION::ResetViewport(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetViewports(1, &viewport);
}