#pragma once

class COLORSHADER
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	COLORSHADER();
	COLORSHADER(const COLORSHADER&);
	~COLORSHADER();

	bool Initialize();
	void Shutdown();
	bool Render(int, RNDMATRIXS );

private:
	bool InitializeShader(WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputErrorMessage(WCHAR*, ID3D10Blob*);

	bool SetShaderParameters( );
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
};
