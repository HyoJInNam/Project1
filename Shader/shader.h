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