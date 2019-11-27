#pragma once

class SHADER;

class LIGHTSHADER : public SHADER
{
	class CameraBufferType
	{
	public:
		D3DXVECTOR3 cameraPosition;
		float padding;
	};
	

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* cameraBuffer;

	ID3D11Buffer* lightBuffer;
public:
	LIGHTSHADER();
	LIGHTSHADER(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	LIGHTSHADER(const LIGHTSHADER&);
	virtual ~LIGHTSHADER() override;

	virtual bool Initialize() override;
	bool Render(int, RNDMATRIXS, D3DXVECTOR3 cameraPosition, LightBufferType* light);
	bool Render(int, RNDMATRIXS, D3DXVECTOR3 cameraPosition, ID3D11ShaderResourceView* texture, LightBufferType* light);


private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(D3DXVECTOR3 cameraPosition, ID3D11ShaderResourceView* texture, LightBufferType* light);

};