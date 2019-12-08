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

	// 정점 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	ISFAILED(result);

	// 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
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
	// 모델 파일을 엽니다.
	ifstream fin;
	fin.open(filename);

	// 파일을 열 수 없으면 종료합니다.
	if (fin.fail())
	{
		return false;
	}

	// 버텍스 카운트의 값까지 읽는다.
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// 버텍스 카운트를 읽는다.
	fin >> vertexCount;

	// 인덱스의 수를 정점 수와 같게 설정합니다.
	indexCount = vertexCount;

	// 읽어 들인 정점 개수를 사용하여 모델을 만듭니다.
	object = new ModelType[vertexCount];
	if (!object)
	{
		return false;
	}

	// 데이터의 시작 부분까지 읽는다.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// 버텍스 데이터를 읽습니다.
	for (int i = 0; i < vertexCount; i++)
	{
		fin >> object[i].x >> object[i].y >> object[i].z;
		fin >> object[i].tu >> object[i].tv;
		fin >> object[i].nx >> object[i].ny >> object[i].nz;
	}

	// 모델 파일을 닫는다.
	fin.close();

	return true;
}