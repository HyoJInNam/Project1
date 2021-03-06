#include "shader.h"
#include "ShadowShaderClass.h"


ShadowShaderClass::ShadowShaderClass(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: SHADER(hwnd, device, deviceContext), 
	lightBuffer2(nullptr)
{}


ShadowShaderClass::ShadowShaderClass(const ShadowShaderClass& other) : SHADER(this) {}
ShadowShaderClass::~ShadowShaderClass() { Shutdown(); }


bool ShadowShaderClass::Initialize()
{
	ISFAILED(InitializeShader(const_cast<WCHAR*>(L"../data/shader/shadow_vs.hlsl"), const_cast<WCHAR*>(L"../data/shader/shadow_ps.hlsl")));
	InitializeShaderBuffer();
	return true;
}



bool ShadowShaderClass::Render(
	int indexCount,
	RNDMATRIXS matrixs,
	ID3D11ShaderResourceView* texture, 
	ID3D11ShaderResourceView* depthMapTexture, 
	D3DXVECTOR3 lightPosition,
	LightBufferType* light)
{
	render = matrixs;
	ISFAIL(SetShaderParameters(matrixs, texture, depthMapTexture, lightPosition, light));
	RenderShader(indexCount);

	return true;
}


bool ShadowShaderClass::InitializeShader(
	WCHAR* vsFilename,
	WCHAR* psFilename)
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "ShadowVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");


	result = D3DCompileFromFile(psFilename, NULL, NULL, "ShadowPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");



	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);


	// 정점 입력 레이아웃 구조체를 설정합니다.
	// 이 설정은 ModelClass와 셰이더의 VertexType 구조와 일치해야합니다.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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



	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout);
	ISFAILED(result);


	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);
	return true;
}


bool ShadowShaderClass::InitializeShaderBuffer()
{
	HRESULT result;
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

		// 텍스처 샘플러 상태를 만듭니다.
		HRESULT result = device->CreateSamplerState(&samplerDesc, &sampleStateWrap);
		ISFAILED(result);


		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;


		result = device->CreateSamplerState(&samplerDesc, &sampleStateClamp);
		ISFAILED(result);
	}


	{
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
		result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
		ISFAILED(result);
	}

	{
		// D3D11_BIND_CONSTANT_BUFFER를 사용하면 ByteWidth가 항상 16의 배수 여야하며 그렇지 않으면 CreateBuffer가 실패합니다.
		D3D11_BUFFER_DESC lightBufferDesc;
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBufferType);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		// 이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듭니다.
		result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
		ISFAILED(result);
	}

	{
		D3D11_BUFFER_DESC lightBufferDesc2;
		lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
		lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc2.MiscFlags = 0;
		lightBufferDesc2.StructureByteStride = 0;

		// 이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듭니다.
		result = device->CreateBuffer(&lightBufferDesc2, NULL, &lightBuffer2);
		ISFAILED(result);
	}

	return true;
}


void ShadowShaderClass::ShutdownShader()
{
	SAFE_RELEASE(lightBuffer2);
	SAFE_RELEASE(lightBuffer);
	SAFE_RELEASE(sampleStateClamp);
	SAFE_RELEASE(sampleStateWrap);
	SAFE_RELEASE(sampleState);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
}



bool ShadowShaderClass::SetShaderParameters(
	RNDMATRIXS matrixs,
	ID3D11ShaderResourceView* texture, 
	ID3D11ShaderResourceView* depthMapTexture, 
	D3DXVECTOR3 lightPosition,
	LightBufferType* light)
{
	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다
	D3DXMatrixTranspose(&render.world, &render.world);
	D3DXMatrixTranspose(&render.view, &render.view);
	D3DXMatrixTranspose(&render.projection, &render.projection);
	D3DXMatrixTranspose(&render.lightView, &render.lightView);
	D3DXMatrixTranspose(&render.lightProjection, &render.lightProjection);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
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

		deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetShaderResources(1, 1, &depthMapTexture);

		// light constant buffer를 잠글 수 있도록 기록한다.
		if (FAILED(deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			return false;
		}
	}
	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = light->ambientColor;
	dataPtr2->diffuseColor = light->diffuseColor;
	dataPtr2->lightDirection = light->lightDirection;

	// 상수 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(lightBuffer, 0);

	// 픽셀 쉐이더에서 광원 상수 버퍼의 위치를 ??설정합니다.
	unsigned int bufferNumber = 0;

	// 마지막으로 업데이트 된 값으로 픽셀 쉐이더에서 광원 상수 버퍼를 설정합니다.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);

	// 쓸 수 있도록 두 번째 빛 상수 버퍼를 잠급니다.
	if(FAILED(deviceContext->Map(lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	LightBufferType2* dataPtr3 = (LightBufferType2*)mappedResource.pData;

	// 조명 변수를 상수 버퍼에 복사합니다.
	dataPtr3->lightPosition = lightPosition;
	dataPtr3->padding = 0.0f;

	// 상수 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(lightBuffer2, 0);

	// 버텍스 쉐이더에서 라이트 상수 버퍼의 위치를 ​​설정합니다.
	bufferNumber = 1;

	// 마지막으로 업데이트 된 값으로 픽셀 쉐이더에서 광원 상수 버퍼를 설정합니다.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &lightBuffer2);

	return true;
}