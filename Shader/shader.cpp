#include "shader.h"




SHADER::SHADER(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext): 
	vertexShader(nullptr),
	pixelShader(nullptr),
	layout(nullptr), 
	sampleState(nullptr), 
	sampleStateWrap(nullptr), 
	sampleStateClamp(nullptr),
	matrixBuffer(nullptr),
	lightBuffer(nullptr), 
	cameraBuffer(nullptr)
{
	this->hwnd = hwnd;
	this->device = device;
	this->deviceContext = deviceContext;
}


void SHADER::OutputErrorMessage(WCHAR* shaderFilename, ID3D10Blob* errorMessage)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();


	SAFE_RELEASE(errorMessage);
	ERR_MESSAGE(L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename);

	return;
}

void SHADER::RenderShader(int indexCount)
{
	deviceContext->IASetInputLayout(layout);

	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	if(sampleState) deviceContext->PSSetSamplers(0, 1, &sampleState);
	if(sampleStateClamp) deviceContext->PSSetSamplers(0, 1, &sampleStateClamp);
	if(sampleStateWrap) deviceContext->PSSetSamplers(0, 1, &sampleStateWrap);

	deviceContext->DrawIndexed(indexCount, 0, 0);

}
