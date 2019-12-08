#include "LoadFile.h"
#include "LoadDomeFile.h"



LOADDOMEFILE::LOADDOMEFILE(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:LOADFILE(device, deviceContext)
{}

LOADDOMEFILE::LOADDOMEFILE(const LOADDOMEFILE&) : LOADFILE(this) {};
LOADDOMEFILE::~LOADDOMEFILE()
{
	ReleaseObject();
}

bool LOADDOMEFILE::Initialize(char* modelFilename)
{
	ISFAIL(LoadFile_txt(modelFilename));
	return InitializeBuffers();
}
void LOADDOMEFILE::Render()
{
	RenderBuffers();
}
void LOADDOMEFILE::Shutdown()
{
	ShutdownBuffers();
}

bool LOADDOMEFILE::InitializeBuffers()
{
	VertexType* vertices = new VertexType[vertexCount];
	ISINSTANCE(vertices);


	unsigned long* indices = new unsigned long[indexCount];
	ISINSTANCE(indices);


	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(object[i].x, object[i].y, object[i].z);
		indices[i] = i;
	}

	// ���� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	ISFAILED(result);

	// �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	ISFAILED(result);


	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}
void LOADDOMEFILE::RenderBuffers()
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;


	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void LOADDOMEFILE::ReleaseObject()
{
	vertexCount = 0;
	indexCount = 0;
	SAFE_DELETE_ARRAY(object);
	return;
}


bool LOADDOMEFILE::LoadFile_txt(char * filename)
{
	// �� ������ ���ϴ�.
	ifstream fin;
	fin.open(filename);

	// ������ �� �� ������ �����մϴ�.
	if (fin.fail())
	{
		return false;
	}

	// ���ؽ� ī��Ʈ�� ������ �д´�.
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// ���ؽ� ī��Ʈ�� �д´�.
	fin >> vertexCount;

	// �ε����� ���� ���� ���� ���� �����մϴ�.
	indexCount = vertexCount;

	// �о� ���� ���� ������ ����Ͽ� ���� ����ϴ�.
	object = new ModelType[vertexCount];
	if (!object)
	{
		return false;
	}

	// �������� ���� �κб��� �д´�.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// ���ؽ� �����͸� �н��ϴ�.
	for (int i = 0; i < vertexCount; i++)
	{
		fin >> object[i].x >> object[i].y >> object[i].z;
		fin >> object[i].tu >> object[i].tv;
		fin >> object[i].nx >> object[i].ny >> object[i].nz;
	}

	// �� ������ �ݴ´�.
	fin.close();

	return true;
}