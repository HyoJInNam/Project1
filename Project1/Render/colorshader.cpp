#include "renafx.h"
#include "colorshader.h"

COLORSHADER::COLORSHADER()
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();

	vertexShader = 0;
	pixelShader = 0;
	layout = 0;
	matrixBuffer = 0;
}


COLORSHADER::COLORSHADER(const COLORSHADER& other) {}
COLORSHADER::~COLORSHADER() {}


bool COLORSHADER::Initialize()
{
	return InitializeShader(const_cast<WCHAR*>(L"./Engine/color.vs"), const_cast<WCHAR*>(L"./Engine/color.ps"));
}


void COLORSHADER::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool COLORSHADER::Render(int indexCount, RNDMatrix matrix)
{
	ISFAIL(SetShaderParameters(matrix));
	RenderShader(indexCount);
	return true;
}


bool COLORSHADER::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL,	"ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, vsFilename, errorMessage, L"Missing Shader File");



    // Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	ISFAILEDFILE(result, psFilename, errorMessage, L"Missing Shader File");


    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	ISFAILED(result);

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	ISFAILED(result);

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
									   vertexShaderBuffer->GetBufferSize(), &layout);

	ISFAILED(result);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(RNDMatrix);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	ISFAILED(result);

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


void COLORSHADER::OutputErrorMessage(WCHAR* shaderFilename, ID3D10Blob* errorMessage)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");

	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	SAFE_RELEASE(errorMessage);
	ERR_MESSAGE(L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename);
	
	return;
}


bool COLORSHADER::SetShaderParameters(RNDMatrix matrix)
{
	//HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;


	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&matrix.world, &matrix.world);
	D3DXMatrixTranspose(&matrix.view, &matrix.view);
	D3DXMatrixTranspose(&matrix.projection, &matrix.projection);

	// Lock the constant buffer so it can be written to.
	ISFAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// Get a pointer to the data in the constant buffer.
	{
		RNDMatrix* dataPtr = (RNDMatrix*)mappedResource.pData;

		dataPtr->world = matrix.world;
		dataPtr->view = matrix.view;
		dataPtr->projection = matrix.projection;

		deviceContext->Unmap(matrixBuffer, 0);

		unsigned bufferNumber = 0;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	}

	return true;
}


void COLORSHADER::RenderShader(int indexCount)
{
	deviceContext->IASetInputLayout(layout);

    deviceContext->VSSetShader(vertexShader, NULL, 0);
    deviceContext->PSSetShader(pixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}