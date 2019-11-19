#include "../../Utility/stdafx.h"

#include "./LoadFile.h"
#include "TextureArrayClass.h"


TextureArrayClass::TextureArrayClass() {}
TextureArrayClass::TextureArrayClass(const TextureArrayClass& other) {}
TextureArrayClass::~TextureArrayClass() {}

bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	LoadTexture(filename);
	textures.push_back(GetTexture());
	return true;
}


void TextureArrayClass::Shutdown()
{
	for (auto &texture : textures)
	{
		ReleaseTexture();
		textures.pop_back();
	}
	return;
}


vector< ID3D11ShaderResourceView* > TextureArrayClass::GetTextureArray()
{
	return textures;
}