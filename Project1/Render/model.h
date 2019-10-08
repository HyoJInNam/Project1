#pragma once

//class TextureClass;
class LOADOBJECTSFILE;

class MODEL
{
public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(char*);
	void Shutdown();
	void Render();


private:
	bool InitializeBuffers();
	void RenderBuffers();
	void ShutdownBuffers();


private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	LOADOBJECTSFILE* loadFile;

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;

public:

	int GetIndexCount();

	//ID3D11ShaderResourceView* GetTexture();
};

