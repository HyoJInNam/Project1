#include "../stdafx.h"
#include "d3dclass.h"


D3D::D3D()
	: numerator(0), denominator(1)
{
	SetGpuInfo();
}
D3D::~D3D()
{
	DeleteBackBuffer();

	if (swapChain)
		swapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
}


void D3D::SetGpuInfo()
{
	HRESULT result;

	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput*  adapterOutput = nullptr;

	unsigned int numModes;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	assert(SUCCEEDED(result));

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(result));

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(result));

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	assert(SUCCEEDED(result));

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	assert(SUCCEEDED(result));

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)wndDesc.screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)wndDesc.screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	{
		result = adapter->GetDesc(&adapterDesc);
		assert(SUCCEEDED(result));

		// Store the dedicated video card memory in megabytes.
		videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		videoCardDescription = adapterDesc.Description;
	}


	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
}
void D3D::CreateSwapChain()
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = wndDesc.screenWidth;
	swapChainDesc.BufferDesc.Height = wndDesc.screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (wndDesc.vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = wndDesc.Handle;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = (wndDesc.fullscreen) ? false : true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;


	D3D_FEATURE_LEVEL featureLevel;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	assert(SUCCEEDED(result));
}
void D3D::CreateBackBuffer()
{

	HRESULT result;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	{
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		assert(SUCCEEDED(result));

		result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		assert(SUCCEEDED(result));

		SAFE_RELEASE(backBufferPtr);
	}


	D3D11_TEXTURE2D_DESC depthBufferDesc;
	{
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = wndDesc.screenWidth;
		depthBufferDesc.Height = wndDesc.screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
		assert(SUCCEEDED(result));
	}

	
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ID3D11DepthStencilState* m_depthStencilState;
	{
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
		assert(SUCCEEDED(result));

		// Set the depth stencil state.
		deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}

	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	{
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		assert(SUCCEEDED(result));

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	}


	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* m_rasterState;
	{
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		result = device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		assert(SUCCEEDED(result));

		// Now set the rasterizer state.
		deviceContext->RSSetState(m_rasterState);
	}


	D3D11_VIEWPORT viewport;
	{
		viewport.Width = (FLOAT)wndDesc.screenWidth;
		viewport.Height = (FLOAT)wndDesc.screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		deviceContext->RSSetViewports(1, &viewport);
	}


	float fieldOfView, screenAspect;

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (FLOAT)wndDesc.screenWidth / (FLOAT)wndDesc.screenHeight;


	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&matrixs.projection, fieldOfView, screenAspect, wndDesc.screenNear, wndDesc.screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&matrixs.world);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&matrixs.ortho, (FLOAT)wndDesc.screenWidth, (FLOAT)wndDesc.screenHeight, wndDesc.screenNear, wndDesc.screenDepth);

}
void D3D::DeleteBackBuffer()
{
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(renderTargetView);

	return;
}


void D3D::BeginScene(D3DXCOLOR color)
{
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}
void D3D::ResizeScene(UINT width, UINT height)
{
	if (width < 800 || height < 600) return;

	wndDesc.screenWidth = width;
	wndDesc.screenHeight = height;

	//DeleteBackBuffer();
	//{
	//	HRESULT hr = swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);
	//	assert(SUCCEEDED(hr));
	//}
	//CreateBackBuffer(width, height);
}
void D3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	swapChain->Present(wndDesc.vsync == true ? 1 : 0, 0);

	return;
}

