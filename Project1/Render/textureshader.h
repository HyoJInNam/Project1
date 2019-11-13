#pragma once

enum ShaderType
{
	COLOR_SAHDER,
	TEXTURE_SHADER
};
class TEXTURESHADER
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	TEXTURESHADER(HWND, ID3D11Device*, ID3D11DeviceContext*);
	TEXTURESHADER(const TEXTURESHADER&);
	~TEXTURESHADER();

	bool Initialize(WCHAR*);
	void Shutdown();
	bool Render(int, RNDMATRIXS);


	ID3D11ShaderResourceView* GetTexture() { return texture; }
private:
	bool InitializeShader(WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputErrorMessage(WCHAR*, ID3D10Blob*);

	bool SetShaderParameters();
	void RenderShader(int);


private:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	RNDMATRIXS  render;
	ShaderType shaderType;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;

	ID3D11SamplerState* m_sampleState;
	ID3D11ShaderResourceView* texture;
};
