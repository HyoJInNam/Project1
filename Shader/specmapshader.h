#pragma once


class SHADER;

class SpecMapShaderClass : public SHADER
{

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 specularColor;
		float specularPower;
		D3DXVECTOR3 lightDirection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};


	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11Buffer* lightBuffer = nullptr;
	ID3D11Buffer* cameraBuffer = nullptr;

public:
	SpecMapShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	SpecMapShaderClass(const SpecMapShaderClass&);
	virtual ~SpecMapShaderClass() override;

	virtual bool Initialize() override;
	bool Render(int, RNDMATRIXS, ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);
};