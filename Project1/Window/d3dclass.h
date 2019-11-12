#pragma once


class D3D: public SINGLETON <D3D>
{
public:
	D3D();
	~D3D();

	void Initialize() {
		CreateSwapChain();
		CreateTextBackBuffer();
		CreateBackBuffer();
	}

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }
	void GetVideoCardInfo(char* cardName, int& memory)
	{
		USES_CONVERSION;
		cardName = W2A(videoCardDescription);
		memory = videoCardMemory;
		return;
	}

private:
	void SetGpuInfo();
	void CreateSwapChain();
	void CreateBackBuffer();
	void CreateTextBackBuffer();
	void DeleteBackBuffer();

public:
	void BeginScene(D3DXCOLOR);
	void EndScene();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
private:
	unsigned int  numerator, denominator;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	int videoCardMemory;
	WCHAR* videoCardDescription;

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState ;
	ID3D11BlendState* m_alphaEnableBlendingState ;
	ID3D11BlendState* m_alphaDisableBlendingState ;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RenderTargetView* renderTargetView;

};