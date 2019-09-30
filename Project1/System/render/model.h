#pragma once

class MODEL
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR4 color;
	};

public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
