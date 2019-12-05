#include "shader.h"
#include "colorshader.h"

COLORSHADER::COLORSHADER(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: SHADER(hwnd, device, deviceContext)
{}

COLORSHADER::COLORSHADER(const COLORSHADER& other): SHADER(this) {}
COLORSHADER::~COLORSHADER() {  }


bool COLORSHADER::Initialize()
{
	shaderType = SAHDER_COLOR;
	InitializeShader(const_cast<WCHAR*>(L"./data/shader/color.vs"), const_cast<WCHAR*>(L"./data/shader/color.ps"));
	InitializeShaderBuffer();
	return true;
}



bool COLORSHADER::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename) {

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;


	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");

	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");


	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);


	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;


	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout);
	ISFAILED(result);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);
	return true;
}
bool COLORSHADER::InitializeShaderBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;
	{
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
		ISFAILED(result);
	}

	return true;
}
bool COLORSHADER::Render(int indexCount, RNDMATRIXS matrixs)
{
	render = matrixs;
	ISFAIL(SetShaderParameters());
	RenderShader(indexCount);

	return true;
}
void COLORSHADER::ShutdownShader()
{
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
	return;
}

bool COLORSHADER::SetShaderParameters()
{
	HRESULT result;


	D3DXMatrixTranspose(&render.world, &render.world);
	D3DXMatrixTranspose(&render.view, &render.view);
	D3DXMatrixTranspose(&render.projection, &render.projection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ISFAILED(result);

	MatrixBufferType* dataPtr;
	{
		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = render.world;
		dataPtr->view = render.view;
		dataPtr->projection = render.projection;

		deviceContext->Unmap(matrixBuffer, 0);

		unsigned int bufferNumber = 0;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	}
	return true;
}