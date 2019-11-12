#pragma once

class TextureClass;

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	bool LoadTexture(ID3D11Device*, WCHAR*);


	void ReleaseFontData() { SAFE_DELETE(m_Font); }
	void ReleaseTexture() { SAFE_RELEASE(m_texture); }

private:
	FontType* m_Font = nullptr;
	ID3D11ShaderResourceView* m_texture;
};