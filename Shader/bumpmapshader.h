#pragma once



class SHADER;

class BumpMapShaderClass : public SHADER
{

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;
	};




	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;


public:
	BumpMapShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	BumpMapShaderClass(const BumpMapShaderClass&);
	virtual ~BumpMapShaderClass() override;

	virtual bool Initialize() override;
	bool Render(int, RNDMATRIXS, ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;
	virtual void ShutdownShader()override;

	virtual bool InitializeShaderBuffer() override;
	bool SetShaderParameters(ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4);
};