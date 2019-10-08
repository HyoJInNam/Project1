#include "renafx.h"
#include "model.h"



MODEL::MODEL()
{
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();
	loadFile = new LOADOBJECTSFILE;
}


MODEL::MODEL(const MODEL& other) {}
MODEL::~MODEL() 
{
	SAFE_DELETE(loadFile);
}


bool MODEL::Initialize(char* modelFilename)
{
	CString m_temp = modelFilename;
	CString temp = m_temp.Right(4);
	if (temp.CompareNoCase(_T(".txt")) == 0) { ISFAIL(loadFile->LoadModel(modelFilename)); }
	//else if	(temp.CompareNoCase(_T(".obj")) == 0) { ISFAIL(LoadObject(modelFilename)); }
	ISFAIL(InitializeBuffers());

	return true;
}
void MODEL::Render()
{
	RenderBuffers();
	return;
}
void MODEL::Shutdown()
{
	ShutdownBuffers();
	return;
}



bool MODEL::InitializeBuffers()
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[loadFile->m_vertexCount];
	ISINSTANCE(vertices);

	// Create the index array.
	indices = new unsigned long[loadFile->m_indexCount];
	ISINSTANCE(indices);

	// Load the vertex array and index array with data.
	for (i = 0; i < loadFile->m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(loadFile->object[i].x, loadFile->object[i].y, loadFile->object[i].z);
		vertices[i].texture = D3DXVECTOR2(loadFile->object[i].tu, loadFile->object[i].tv);
		vertices[i].normal = D3DXVECTOR3(loadFile->object[i].nx, loadFile->object[i].ny, loadFile->object[i].nz);

		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * loadFile->m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	ISFAILED(result);

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * loadFile->m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	ISFAILED(result);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}


void MODEL::ShutdownBuffers()
{
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);
	return;
}

int MODEL::GetIndexCount() { return loadFile->m_indexCount; }


void MODEL::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
