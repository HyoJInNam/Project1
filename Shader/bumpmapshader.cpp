#include "shader.h"
#include "bumpmapshader.h"


BumpMapShaderClass::BumpMapShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: SHADER(hwnd, device, deviceContext)
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();
}


BumpMapShaderClass::BumpMapShaderClass(const BumpMapShaderClass& other): SHADER(this){}
BumpMapShaderClass::~BumpMapShaderClass() {}


bool BumpMapShaderClass::Initialize()
{
	InitializeShader(const_cast<WCHAR*>(L"./data/shader/bumpmap.vs"), const_cast<WCHAR*>(L"./data/shader/bumpmap.ps"));
	InitializeShaderBuffer();
	return true;
}



bool BumpMapShaderClass::Render(
	int indexCount, 
	RNDMATRIXS matrixs,
	D3DXVECTOR3 cameraPosition,
	ID3D11ShaderResourceView** textureArray,
	LightBufferType* light)
{
	render = matrixs;
	ISFAIL(SetShaderParameters(cameraPosition, textureArray, light));
	RenderShader(indexCount);

	return true;
}


bool BumpMapShaderClass::InitializeShader(
	WCHAR* vsFilename, 
	WCHAR* psFilename)
{
	ID3D10Blob* errorMessage = nullptr;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;


	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "BumpMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,	&vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");


	result = D3DCompileFromFile(psFilename, NULL, NULL, "BumpMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);


	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);



	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;


	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout);
	ISFAILED(result);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	
	return true;
}


void BumpMapShaderClass::ShutdownShader()
{
	if (!this) return;
	SAFE_RELEASE(cameraBuffer);
	SAFE_RELEASE(lightBuffer);
	SAFE_RELEASE(sampleState);
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
}


bool BumpMapShaderClass::InitializeShaderBuffer()
{
	HRESULT result;

	{
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;


		result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
		ISFAILED(result);
	}

	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


		result = device->CreateSamplerState(&samplerDesc, &sampleState);
		ISFAILED(result);
	}

	{
		D3D11_BUFFER_DESC lightBufferDesc;
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBufferType);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
		ISFAILED(result);
	}

	{
		D3D11_BUFFER_DESC cameraBufferDesc;
		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		result = device->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);
		ISFAILED(result);
	}
	return true;
}


bool BumpMapShaderClass::SetShaderParameters(
	D3DXVECTOR3 cameraPosition,
	ID3D11ShaderResourceView** textureArray, 
	LightBufferType* light)
{

	HRESULT result;

	D3DXMatrixTranspose(&render.world, &render.world);
	D3DXMatrixTranspose(&render.view, &render.view);
	D3DXMatrixTranspose(&render.projection, &render.projection);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ISFAILED(result);


	{
		MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
		dataPtr->world = render.world;
		dataPtr->view = render.view;
		dataPtr->projection = render.projection;

		deviceContext->Unmap(matrixBuffer, 0);

		unsigned int bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
		ISFAILED(deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	}

	{
		CameraBufferType* dataPtr2 = (CameraBufferType*)mappedResource.pData;
		dataPtr2->cameraPosition = cameraPosition;
		dataPtr2->padding = 0.0f;

		deviceContext->Unmap(cameraBuffer, 0);
		unsigned int bufferNumber = 1;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraBuffer);
		deviceContext->PSSetShaderResources(0, 2, textureArray);
		result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		ISFAILED(result);
	}

	{
		LightBufferType* dataPtr3 = (LightBufferType*)mappedResource.pData;
		dataPtr3->lightDirection = light->lightDirection;
		dataPtr3->ambientColor = light->ambientColor;
		dataPtr3->diffuseColor = light->diffuseColor;

		deviceContext->Unmap(lightBuffer, 0);
		unsigned int bufferNumber = 0;

		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);
	}



	return true;
}

