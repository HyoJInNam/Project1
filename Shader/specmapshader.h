#pragma once


class SHADER;

class SpecMapShaderClass : public SHADER
{
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11Buffer* lightBuffer = nullptr;
	ID3D11Buffer* cameraBuffer = nullptr;

public:
	SpecMapShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	SpecMapShaderClass(const SpecMapShaderClass&);
	virtual ~SpecMapShaderClass() override;

	virtual bool Initialize() override;
	bool Render(
		int indexCount,
		RNDMATRIXS render,
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView** textureArray,
		D3DXVECTOR3 lightDirection,
		LightBufferType* light);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView** textureArray,
		D3DXVECTOR3 lightDirection,
		LightBufferType* light);
};