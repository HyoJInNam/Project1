#include "shader.h"
#include "domeshader.h"


DOMESHADER::DOMESHADER(
	HWND hwnd,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext)
	: SHADER(hwnd, device, deviceContext)
{}
DOMESHADER::DOMESHADER(const DOMESHADER& other) : SHADER(this) {}
DOMESHADER::~DOMESHADER() {}


bool DOMESHADER::Initialize()
{
	ISFAILED(InitializeShader(
		const_cast<WCHAR*>(L"../data/system/skydome_vs.hlsl"),
		const_cast<WCHAR*>(L"../data/system/skydome_ps.hlsl")));
	InitializeShaderBuffer();
	return true;
}


bool DOMESHADER::Render(
	int indexCount,
	RNDMATRIXS& matrixs,
	GradientBufferType& color)
{
	render = matrixs;
	ISFAIL(SetShaderParameters(matrixs, color));
	RenderShader(indexCount);
	return true;
}


bool DOMESHADER::InitializeShader(
	WCHAR* vsFilename, 
	WCHAR* psFilename)
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "SkyDomeVertexShader", "vs_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");

	result = D3DCompileFromFile(psFilename, NULL, NULL, "SkyDomePixelShader", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);

	// 정점 입력 레이아웃 구조체를 설정합니다.
	// 이 설정은 ModelClass와 셰이더의 VertexType 구조와 일치해야합니다.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// 레이아웃의 요소 수를 가져옵니다.
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);
	ISFAILED(result);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	return true;
}

bool DOMESHADER::InitializeShaderBuffer()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	ISFAILED(result);

	// 픽셀 쉐이더에있는 그라데이션 동적 상수 버퍼의 설명을 설정합니다.
	// D3D11_BIND_CONSTANT_BUFFER를 사용하면 ByteWidth가 항상 16의 배수 여야하며 그렇지 않으면 CreateBuffer가 실패합니다.
	D3D11_BUFFER_DESC gradientBufferDesc;
	gradientBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDesc.ByteWidth = sizeof(GradientBufferType);
	gradientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDesc.MiscFlags = 0;
	gradientBufferDesc.StructureByteStride = 0;


	result = device->CreateBuffer(&gradientBufferDesc, NULL, &gradientBuffer);
	ISFAILED(result);

	return true;
}

void DOMESHADER::ShutdownShader()
{
	SAFE_RELEASE(gradientBuffer);
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
}

bool DOMESHADER::SetShaderParameters(
	RNDMATRIXS& matrixs,
	GradientBufferType& color)
{
	D3DXMatrixTranspose(&render.world, &render.world);
	D3DXMatrixTranspose(&render.view, &render.view);
	D3DXMatrixTranspose(&render.projection, &render.projection);
	D3DXMatrixTranspose(&render.lightView, &render.lightView);
	D3DXMatrixTranspose(&render.lightProjection, &render.lightProjection);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ISFAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

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
		ISFAILED(deviceContext->Map(gradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	}

	GradientBufferType* dataPtr2 = (GradientBufferType*)mappedResource.pData;
	{
		dataPtr2->topColor = color.topColor;
		dataPtr2->centerColor = color.centerColor;
		dataPtr2->bottomColor = color.bottomColor;

		// 상수 버퍼의 잠금을 해제합니다.
		deviceContext->Unmap(gradientBuffer, 0);
		unsigned int bufferNumber = 0;
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &gradientBuffer);

	}
	return true;
}