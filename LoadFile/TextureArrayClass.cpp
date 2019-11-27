#include "LoadFile.h"
#include "TextureArrayClass.h"


TextureArray::TextureArray(){}
TextureArray::TextureArray(const TextureArray& other) {}
TextureArray::~TextureArray() {}


bool TextureArray::InitializeTextureArray(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	if (filename1) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_textures[0], NULL));
	if (filename2) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL));
	if (filename3) ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename3, NULL, NULL, &m_textures[2], NULL));

	return true;
}


void TextureArray::ShutdownTextureArray()
{
	if (m_textures[0]) SAFE_RELEASE(m_textures[0]);
	if (m_textures[1]) SAFE_RELEASE(m_textures[1]);
	if (m_textures[2]) SAFE_RELEASE(m_textures[2]);
	return;
}


ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return m_textures;
}