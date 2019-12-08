#include "shader.h"
#include "depthshaderclass.h"


DepthShaderClass::DepthShaderClass(
	HWND hwnd,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext)
	: SHADER(hwnd, device, deviceContext)
{}

DepthShaderClass::DepthShaderClass(const DepthShaderClass& other) : SHADER(this) {}
DepthShaderClass::~DepthShaderClass() {  }


bool DepthShaderClass::Initialize()
{
	ISFAILED(InitializeShader(
		const_cast<WCHAR*>(L"./data/system/depth.vs"),
		const_cast<WCHAR*>(L"./data/system/depth.ps")));
	InitializeShaderBuffer();
	return true;
}



void DepthShaderClass::ShutdownShader()
{
	SAFE_RELEASE(layout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
}

bool DepthShaderClass::Render(
	int indexCount,
	RNDMATRIXS& matrixs)
{
	render = matrixs;
	ISFAIL(SetShaderParameters(matrixs));
	RenderShader(indexCount);

	return true;
}


bool DepthShaderClass::InitializeShader(
	WCHAR* vsFilename, 
	WCHAR* psFilename)
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

    // Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "DepthVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");


	result = D3DCompileFromFile(psFilename, NULL, NULL, "DepthPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");



    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);

    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;


	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);
	ISFAILED(result);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);
	return true;
}


bool DepthShaderClass::InitializeShaderBuffer()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	ISFAILED(result);

	return true;
}

bool DepthShaderClass::SetShaderParameters(RNDMATRIXS matrixs)
{
	D3DXMatrixTranspose(&render.world, &render.world);
	D3DXMatrixTranspose(&render.view, &render.view);
	D3DXMatrixTranspose(&render.projection, &render.projection);
	D3DXMatrixTranspose(&render.lightView, &render.lightView);
	D3DXMatrixTranspose(&render.lightProjection, &render.lightProjection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ISFAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	{

		dataPtr->world = render.world;
		dataPtr->view = render.view;
		dataPtr->projection = render.projection;
		dataPtr->lightView = render.lightView;
		dataPtr->lightProjection = render.lightProjection;


		deviceContext->Unmap(matrixBuffer, 0);
		unsigned int bufferNumber = 0;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	}
	return true;
}
