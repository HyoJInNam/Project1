#pragma once

class TextureClass;

class MODEL
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(char*);
	void Shutdown();
	void Render();

	int GetIndexCount() { return indexCount; }
private:
	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();
	
	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int vertexCount, indexCount;
	ModelType* model;
};

