#pragma once

class BumpMapShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

public:
	BumpMapShaderClass();
	BumpMapShaderClass(const BumpMapShaderClass&);
	~BumpMapShaderClass();

	bool Initialize();
	void Shutdown();
	bool Render(int, RNDMATRIXS, ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4);

private:
	bool InitializeShader(WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputErrorMessage(WCHAR*, ID3D10Blob*);

	bool SetShaderParameters(ID3D11ShaderResourceView**, D3DXVECTOR3, D3DXVECTOR4);
	void RenderShader(int);

private:

	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	RNDMATRIXS  render;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};