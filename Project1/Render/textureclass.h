#pragma once

class TEXTURE
{
public:
	TEXTURE(ID3D11Device*);
	TEXTURE(const TEXTURE&);
	~TEXTURE();

	bool Initialize(WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture() { return texture; }

private:
	ID3D11Device* device;

	ID3D11ShaderResourceView* texture;
};