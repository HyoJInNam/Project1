#pragma once

#include <Utility.h>

#include <fstream>
#include <vector>
using namespace std;


enum ShaderType
{
	SAHDER_COLOR,
	SHADER_TEXTURE,
	SHADER_LIGHT,
	SHADER_BUMP,
	SHADER_SPECULAR
};

struct MatrixBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX lightView;
	D3DXMATRIX lightProjection;
};

struct LightBufferType
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float specularPower;
	D3DXVECTOR4 specularColor;
};

struct LightBufferType2
{
	D3DXVECTOR3 lightPosition;
	float padding;
};

struct CameraBufferType
{
	D3DXVECTOR3 cameraPosition;
	float padding;
};

struct GradientBufferType
{
	D3DXVECTOR4 topColor;
	D3DXVECTOR4 centerColor;
	D3DXVECTOR4 bottomColor;
};

class SHADER {

protected:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	RNDMATRIXS  render;

	ShaderType shaderType;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateWrap;
	ID3D11SamplerState* sampleStateClamp;

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;

protected:
	SHADER(HWND, ID3D11Device*, ID3D11DeviceContext*);
	SHADER(const SHADER*) {};
	virtual ~SHADER() {}

	virtual bool InitializeShader(WCHAR*, WCHAR*) = 0;

	virtual bool InitializeShaderBuffer() = 0;
	virtual void ShutdownShader() = 0;

	void OutputErrorMessage(WCHAR* shaderFilename, ID3D10Blob* errorMessage);



public:
	virtual bool Initialize() = 0;
	void RenderShader(int indexCount);
	void Shutdown() { ShutdownShader(); }
};