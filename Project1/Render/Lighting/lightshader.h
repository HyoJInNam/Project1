#pragma once

class MODEL;
class LIGHT;

class LIGHTSHADER: public LIGHT
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	class CameraBufferType
	{
	public:
		D3DXVECTOR3 cameraPosition;
		float padding;
	};
		
public:
	LIGHTSHADER();
	LIGHTSHADER(const LIGHTSHADER&);
	~LIGHTSHADER();

	bool Initialize();
	void Shutdown();
	bool Render(int, RNDMATRIXS, D3DXVECTOR3 cameraPosition);
	bool Render(int, RNDMATRIXS, D3DXVECTOR3 cameraPosition, ID3D11ShaderResourceView* texture);

	LightBufferType GetLight() { return *light; }

private:
	bool InitializeShader(WCHAR*, WCHAR*);
	bool InitializeShaderBuffer();
	void ShutdownShader();
	void OutputErrorMessage(WCHAR*, ID3D10Blob*);

	bool SetShaderParameters(D3DXVECTOR3 cameraPosition, ID3D11ShaderResourceView* texture);
	void RenderShader(int);

private:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	RNDMATRIXS  render;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11SamplerState* sampleState;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* cameraBuffer;

	ID3D11Buffer* lightBuffer;
};
