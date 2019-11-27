#pragma once

class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool InitializeTextureArray(ID3D11Device*device,
		WCHAR* filename1 = nullptr,
		WCHAR* filename2 = nullptr,
		WCHAR* filename3 = nullptr);

	void ShutdownTextureArray();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[3] = { nullptr,  };
};