
#include "LoadFile.h"


LOADFILE::LOADFILE(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:vertexBuffer(nullptr), indexBuffer(nullptr)
	, vertexCount(0), indexCount(0)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

void LOADFILE::ShutdownBuffers()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	return;
}






bool LOADFILE::InitializeTextureArray(ID3D11Device * device, WCHAR * texture, WCHAR * normalmap, WCHAR * specularmap)
{
	if (texture) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, texture, NULL, NULL, &m_textures[0], NULL));
	if (normalmap) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, normalmap, NULL, NULL, &m_textures[1], NULL));
	if (specularmap) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, specularmap, NULL, NULL, &m_textures[2], NULL));

	return true;
}

void LOADFILE::ShutdownTextureArray()
{
	if (m_textures[0]) SAFE_RELEASE(m_textures[0]);
	if (m_textures[1]) SAFE_RELEASE(m_textures[1]);
	if (m_textures[2]) SAFE_RELEASE(m_textures[2]);
}

bool LOADFILE::LoadTexture(ID3D11Device * device, WCHAR * texture, WCHAR * normalmap, WCHAR * specularmap)
{
	return InitializeTextureArray(device, texture, normalmap, specularmap);
}
