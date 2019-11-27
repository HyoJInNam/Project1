
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






bool LOADFILE::InitializeTextureArray(ID3D11Device * device, WCHAR * texture_file_name, WCHAR * normal_texture_name, WCHAR * filename3)
{
	if (texture_file_name) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, texture_file_name, NULL, NULL, &m_textures[0], NULL));
	if (normal_texture_name) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, normal_texture_name, NULL, NULL, &m_textures[1], NULL));
	if (filename3) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename3, NULL, NULL, &m_textures[2], NULL));

	return true;
}

void LOADFILE::ShutdownTextureArray()
{
	if (m_textures[0]) SAFE_RELEASE(m_textures[0]);
	if (m_textures[1]) SAFE_RELEASE(m_textures[1]);
	if (m_textures[2]) SAFE_RELEASE(m_textures[2]);
}

bool LOADFILE::LoadTexture(ID3D11Device * device, WCHAR * texture_file_name, WCHAR * normal_texture_name, WCHAR * filename3)
{
	return InitializeTextureArray(device, texture_file_name, normal_texture_name, filename3);
}
