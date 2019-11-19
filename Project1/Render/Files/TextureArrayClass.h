#pragma once

class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool InitializeTextureArray(ID3D11Device*, WCHAR*, WCHAR*);
	void ShutdownTextureArray();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[2] = { nullptr, nullptr };
};