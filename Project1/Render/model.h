#pragma once

//class TextureClass;

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

	typedef struct
	{
		float x, y, z;
	}VertexTypeF;

	typedef struct
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	}FaceType;

public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(char*);
	void Shutdown();
	void Render();

	int GetIndexCount() { return m_indexCount; }
	//ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();
	
	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadObject(char* filename);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* model;
};

