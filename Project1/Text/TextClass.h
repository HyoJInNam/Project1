#pragma once

class FontClass;
class FontShaderClass;

class TextClass : public AlignedAllocationPolicy<16>
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(D3DXMATRIX);
	void Shutdown();
	bool Render(D3DXMATRIX, D3DXMATRIX);

	bool SetFps(int);
	bool SetCpu(int);

private:
	bool InitializeSentence(SentenceType**, int);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;


	FontClass* m_Font = nullptr;
	FontShaderClass* m_FontShader = nullptr;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};