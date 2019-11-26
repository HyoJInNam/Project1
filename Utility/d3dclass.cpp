
#include "Utility.h"
#include "wndDescription.h"
#include "d3dclass.h"


D3D::D3D()
	: numerator(0), denominator(1)
{
	wndDesc = WNDDesc::GetInstance();
	SetGpuInfo();
}
D3D::~D3D() {	Shutdown(); }

void D3D::Initialize()
{
	CreateSwapChain();
	CreateTextBackBuffer();
	CreateBackBuffer();
}
void D3D::Shutdown()
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
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	assert(SUCCEEDED(result));

	IDXGIAdapter* adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(result));

	IDXGIOutput* adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(result));

	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	assert(SUCCEEDED(result));

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	assert(SUCCEEDED(result));

	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)wndDesc->sceneWidth)
		{
			if (displayModeList[i].Height == (unsigned int)wndDesc->sceneHeight)
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

		videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
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


	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = wndDesc->sceneWidth;
	swapChainDesc.BufferDesc.Height = wndDesc->sceneHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (wndDesc->vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = wndDesc->Handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = (wndDesc->fullscreen) ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	{
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_DRIVER_TYPE driverTypes[] = 
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		D3D_FEATURE_LEVEL featureLevel;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE g_driverType = driverTypes[driverTypeIndex];
			result = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels),
										D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &deviceContext);
			if (result == E_INVALIDARG)
			{
				result = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, creationFlags, &featureLevels[1], ARRAYSIZE(featureLevels),
											D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &deviceContext);

			}
			assert(SUCCEEDED(result));
		}
		assert(SUCCEEDED(result));
	}
}
void D3D::CreateTextBackBuffer()
{
	HRESULT result;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* TextBackBufferPtr;
	D3D11_TEXTURE2D_DESC depthTextBufferDesc;
	{
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&TextBackBufferPtr);
		assert(SUCCEEDED(result));

		result = device->CreateRenderTargetView(TextBackBufferPtr, NULL, &renderTargetView);
		assert(SUCCEEDED(result));

		SAFE_RELEASE(TextBackBufferPtr);

		ZeroMemory(&depthTextBufferDesc, sizeof(depthTextBufferDesc));

		depthTextBufferDesc.Width = wndDesc->sceneWidth;
		depthTextBufferDesc.Height = wndDesc->sceneHeight;
		depthTextBufferDesc.MipLevels = 1;
		depthTextBufferDesc.ArraySize = 1;
		depthTextBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextBufferDesc.SampleDesc.Count = 1;
		depthTextBufferDesc.SampleDesc.Quality = 0;
		depthTextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextBufferDesc.CPUAccessFlags = 0;
		depthTextBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = device->CreateTexture2D(&depthTextBufferDesc, NULL, &depthStencilBuffer);
		assert(SUCCEEDED(result));
	}
}
void D3D::CreateBackBuffer()
{
	HRESULT result;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	{
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		assert(SUCCEEDED(result));

		result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		assert(SUCCEEDED(result));

		SAFE_RELEASE(backBufferPtr);

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = wndDesc->sceneWidth;
		depthBufferDesc.Height = wndDesc->sceneHeight;
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
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
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

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	{
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
		assert(SUCCEEDED(result));
	}


	D3D11_BLEND_DESC blendStateDescription;
	{
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.AlphaToCoverageEnable = FALSE;
		blendStateDescription.IndependentBlendEnable = FALSE;
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		result = device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
		assert(SUCCEEDED(result));

		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		result = device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
		assert(SUCCEEDED(result));
	}
}
void D3D::DeleteBackBuffer()
{
	SAFE_RELEASE(m_alphaDisableBlendingState);
	SAFE_RELEASE(m_alphaEnableBlendingState);
	SAFE_RELEASE(m_depthDisabledStencilState);
	SAFE_RELEASE(m_depthStencilState);

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

void D3D::EndScene()
{
	swapChain->Present(wndDesc->vsync == TRUE ? 1 : 0, 0);

	return;
}

void D3D::TurnZBufferOn()
{
	deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3D::TurnZBufferOff()
{
	deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void D3D::TurnOnAlphaBlending()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3D::TurnOffAlphaBlending()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

