#include "renafx.h"
#include "textureclass.h"


TEXTURE::TEXTURE(ID3D11Device* device)
	: texture(nullptr)
{
	this->device = device;
}


TEXTURE::TEXTURE(const TEXTURE& other){}
TEXTURE::~TEXTURE() {}


bool TEXTURE::Initialize(WCHAR* filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL);

	ISFAILED(result);
	return true;
}


void TEXTURE::Shutdown()
{
	SAFE_RELEASE(texture);
	return;
}