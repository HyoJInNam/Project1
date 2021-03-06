#pragma once

class WNDDesc;

class D3D: public SINGLETON <D3D>
{
	WNDDesc* wndDesc;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	unsigned int  numerator, denominator;

public:
	D3D();
	~D3D();

	void Initialize();
	void Shutdown();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }
	void GetVideoCardInfo(char* cardName, int& memory)
	{
		USES_CONVERSION;
		cardName = W2A(videoCardDescription);
		memory = videoCardMemory;
		return;
	}

	void SetRenderTarget();
	ID3D11RenderTargetView* GetRenderTargetview() { return this->renderTargetView; }
	void ClearRenderTarget(float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView() { return this->shaderResourceView; }
	void SetBackBufferRenderTarget();


private:
	void SetGpuInfo();
	void CreateSwapChain();
	void CreateBackBuffer();
	void CreateTextBackBuffer();
	void CreateTextureBackBuffer();
	void DeleteBackBuffer();

public:
	void BeginScene(D3DXCOLOR);
	void EndScene();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void TurnOnCulling();
	void TurnOffCulling();



private:

	int videoCardMemory;
	WCHAR* videoCardDescription;




	ID3D11Texture2D* depthStencilBuffer;
	ID3D11ShaderResourceView* shaderResourceView;


	ID3D11RenderTargetView* renderTargetView;
	ID3D11RasterizerState* rasterState;
	ID3D11RasterizerState* rasterStateNoCulling;


	ID3D11DepthStencilView* depthStencilView;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState ;


	ID3D11BlendState* m_alphaEnableBlendingState ;
	ID3D11BlendState* m_alphaDisableBlendingState ;

};