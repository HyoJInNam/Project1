#include "../../Utility/stdafx.h"

#include "../Files/LoadFile.h"
#include "../Files/loadObjFile.h"
#include "../Files/TextureArrayClass.h"

#include "BumpMapping.h"

#include <fstream>
using namespace std;


BUMPMAPPING::BUMPMAPPING(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:LOADOBJECTSFILE(device, deviceContext)
{
}
BUMPMAPPING::BUMPMAPPING(const BUMPMAPPING& other) : LOADOBJECTSFILE(this->device, this->deviceContext) {}
BUMPMAPPING::~BUMPMAPPING() {}

bool BUMPMAPPING::Initialize(char* modelFilename, WCHAR* filename1, WCHAR* filename2){

	CString m_temp = modelFilename;
	CString temp = m_temp.Right(4);
	if (temp.CompareNoCase(_T(".txt")) == 0) { ISFAIL(LoadModel(modelFilename)); }

	CalculateModelVectors();

	ISFAIL(InitializeBuffers());

	textures = new TextureArray;
	ISINSTANCE(textures);
	return LoadTextures(this->device, filename1, filename2);
}


void BUMPMAPPING::Shutdown()
{
	ShutdownBuffers();
	ReleaseModel();
	textures->ShutdownTextureArray();
}
void BUMPMAPPING::Render() { RenderBuffers(); }
//int BUMPMAPPING::GetIndexCount() { return indexCount; }


ID3D11ShaderResourceView** BUMPMAPPING::GetTextures()
{
	return textures->GetTextureArray();
}


bool BUMPMAPPING::InitializeBuffers()
{
	BUMPMAPVertexType* vertices;
	unsigned long* indices;


	vertices = new BUMPMAPVertexType[vertexCount];
	ISINSTANCE(vertices);

	// Create the index array.
	indices = new unsigned long[indexCount];
	ISINSTANCE(indices);

	// ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = D3DXVECTOR2(model[i].tu, model[i].tv);
		vertices[i].normal = D3DXVECTOR3(model[i].nx, model[i].ny, model[i].nz);
		vertices[i].tangent = D3DXVECTOR3(model[i].tx, model[i].ty, model[i].tz);
		vertices[i].binormal = D3DXVECTOR3(model[i].bx, model[i].by, model[i].bz);

		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BUMPMAPVertexType) * vertexCount;
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

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
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
void BUMPMAPPING::RenderBuffers()
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(BUMPMAPVertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}



bool BUMPMAPPING::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	return textures->InitializeTextureArray(device, filename1, filename2);
}


//void BUMPMAPPING::ReleaseTextures()
//{
//	// �ؽ�ó �迭 ������Ʈ�� �������Ѵ�.
//	if (m_TextureArray)
//	{
//		m_TextureArray->Shutdown();
//		delete m_TextureArray;
//		m_TextureArray = 0;
//	}
//}

bool BUMPMAPPING::LoadModel(char* filename)
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
	model = new BUMPMAPModelType[vertexCount];
	if (!model)
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
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	// �� ������ �ݴ´�.
	fin.close();

	return true;
}


void BUMPMAPPING::ReleaseModel()
{
	vertexCount = 0;
	indexCount = 0;
	SAFE_DELETE_ARRAY(model);
}


void BUMPMAPPING::CalculateModelVectors()
{
	BUMPMAPTempVertexType vertex1, vertex2, vertex3;
	BUMPMAPVectorType tangent, binormal, normal;


	// ���� �� ���� ����մϴ�.
	int faceCount = vertexCount / 3;

	// �� �����Ϳ� ���� �ε����� �ʱ�ȭ�մϴ�.
	int index = 0;

	// ������ ���캸�� ����, ����� �� ���� ���͸� ����մϴ�.
	for(int i=0; i<faceCount; i++)
	{
		// �𵨿����̸鿡 ���� �� ���� ������ �����ɴϴ�.
		vertex1.x = model[index].x;
		vertex1.y = model[index].y;
		vertex1.z = model[index].z;
		vertex1.tu = model[index].tu;
		vertex1.tv = model[index].tv;
		vertex1.nx = model[index].nx;
		vertex1.ny = model[index].ny;
		vertex1.nz = model[index].nz;
		index++;

		vertex2.x = model[index].x;
		vertex2.y = model[index].y;
		vertex2.z = model[index].z;
		vertex2.tu = model[index].tu;
		vertex2.tv = model[index].tv;
		vertex2.nx = model[index].nx;
		vertex2.ny = model[index].ny;
		vertex2.nz = model[index].nz;
		index++;

		vertex3.x = model[index].x;
		vertex3.y = model[index].y;
		vertex3.z = model[index].z;
		vertex3.tu = model[index].tu;
		vertex3.tv = model[index].tv;
		vertex3.nx = model[index].nx;
		vertex3.ny = model[index].ny;
		vertex3.nz = model[index].nz;
		index++;

		// ǥ���� ź��Ʈ�� ���� ����� ����մϴ�.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// ������ binormal�� ����Ͽ� �� ������ ����մϴ�.
		CalculateNormal(tangent, binormal, normal);

		// �� �������� ���� ����, ���� �� ���� ����� �����մϴ�.
		model[index-1].nx = normal.x;
		model[index-1].ny = normal.y;
		model[index-1].nz = normal.z;
		model[index-1].tx = tangent.x;
		model[index-1].ty = tangent.y;
		model[index-1].tz = tangent.z;
		model[index-1].bx = binormal.x;
		model[index-1].by = binormal.y;
		model[index-1].bz = binormal.z;

		model[index-2].nx = normal.x;
		model[index-2].ny = normal.y;
		model[index-2].nz = normal.z;
		model[index-2].tx = tangent.x;
		model[index-2].ty = tangent.y;
		model[index-2].tz = tangent.z;
		model[index-2].bx = binormal.x;
		model[index-2].by = binormal.y;
		model[index-2].bz = binormal.z;

		model[index-3].nx = normal.x;
		model[index-3].ny = normal.y;
		model[index-3].nz = normal.z;
		model[index-3].tx = tangent.x;
		model[index-3].ty = tangent.y;
		model[index-3].tz = tangent.z;
		model[index-3].bx = binormal.x;
		model[index-3].by = binormal.y;
		model[index-3].bz = binormal.z;
	}
}


void BUMPMAPPING::CalculateTangentBinormal(BUMPMAPTempVertexType vertex1, BUMPMAPTempVertexType vertex2, BUMPMAPTempVertexType vertex3,
	BUMPMAPVectorType& tangent, BUMPMAPVectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];


	// ���� ǥ���� �� ���͸� ����մϴ�.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// tu �� tv �ؽ�ó ���� ���͸� ����մϴ�.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// ź��Ʈ / ���� ��� �������� �и� ����մϴ�.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// ���� ���� ����ϰ� ����� ���Ͽ� ������ �� ������ ����ϴ�.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// �� ������ ���̸� ����մϴ�.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// ������ ǥ��ȭ �� ���� �����մϴ�.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// �� ������ ���̸� ����մϴ�.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// ������ ǥ��ȭ �� ���� �����մϴ�.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}


void BUMPMAPPING::CalculateNormal(BUMPMAPVectorType tangent, BUMPMAPVectorType binormal, BUMPMAPVectorType& normal)
{
	// ���� ���͸� �� ���ִ� ������ binormal�� ������ ����մϴ�.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// ������ ���̸� ����մϴ�.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// ������ ǥ��ȭ�մϴ�.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}