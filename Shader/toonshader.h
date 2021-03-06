#pragma once

class SHADER;

class TOONSHADER : public SHADER
{
public:
	TOONSHADER();
	TOONSHADER(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	TOONSHADER(const TOONSHADER&);
	virtual ~TOONSHADER() override;

	virtual bool Initialize() override;

	bool Render(
		int indexCount,
		RNDMATRIXS matrixs,
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView* texture = nullptr,
		LightBufferType* light = nullptr);


private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView* texture,
		LightBufferType* light);

};
