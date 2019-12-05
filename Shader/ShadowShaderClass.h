#pragma once


class SHADER;

class ShadowShaderClass : public SHADER
{
	ID3D11Buffer* lightBuffer2 = nullptr;


public:
	ShadowShaderClass(
		HWND hwnd, 
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext);
	ShadowShaderClass(const ShadowShaderClass&);
	virtual ~ShadowShaderClass()override;

	virtual bool Initialize() override;
	bool Render(
		int indexCount,
		RNDMATRIXS matrixs,
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* depthMapTexture,
		D3DXVECTOR3 lightPosition,
		LightBufferType* light);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;
	bool SetShaderParameters(
		RNDMATRIXS matrixs,
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* depthMapTexture,
		D3DXVECTOR3 lightPosition,
		LightBufferType* light);

};