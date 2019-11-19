#include "../../Utility/stdafx.h"

#include "TextureArrayClass.h"


TextureArray::TextureArray(){}
TextureArray::TextureArray(const TextureArray& other) {}
TextureArray::~TextureArray() {}


bool TextureArray::InitializeTextureArray(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_textures[0], NULL));
	ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL));

	return true;
}
bool TextureArray::InitializeTextureArray(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_textures[0], NULL));
	ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL));
	ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[2], NULL));

	return true;
}


void TextureArray::ShutdownTextureArray()
{
	// 텍스처 리소스를 해제한다.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	return;
}


ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return m_textures;
}