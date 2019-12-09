#pragma once



class SHADER;

class BumpMapShaderClass : public SHADER
{
	
public:
	BumpMapShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	BumpMapShaderClass(const BumpMapShaderClass&);
	virtual ~BumpMapShaderClass() override;

	virtual bool Initialize() override;
	bool ToonInitialize();
	bool Render(
		int indexCount,
		RNDMATRIXS matrixs,
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView** textureArray,
		LightBufferType* light);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;
	virtual void ShutdownShader()override;

	virtual bool InitializeShaderBuffer() override;
	bool SetShaderParameters(
		D3DXVECTOR3 cameraPosition,
		ID3D11ShaderResourceView** textureArray,
		LightBufferType* light);
};