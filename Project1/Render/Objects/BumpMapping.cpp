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

	// 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = D3DXVECTOR2(model[i].tu, model[i].tv);
		vertices[i].normal = D3DXVECTOR3(model[i].nx, model[i].ny, model[i].nz);
		vertices[i].tangent = D3DXVECTOR3(model[i].tx, model[i].ty, model[i].tz);
		vertices[i].binormal = D3DXVECTOR3(model[i].bx, model[i].by, model[i].bz);

		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BUMPMAPVertexType) * vertexCount;
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

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
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
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(BUMPMAPVertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}



bool BUMPMAPPING::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	return textures->InitializeTextureArray(device, filename1, filename2);
}


//void BUMPMAPPING::ReleaseTextures()
//{
//	// 텍스처 배열 오브젝트를 릴리즈한다.
//	if (m_TextureArray)
//	{
//		m_TextureArray->Shutdown();
//		delete m_TextureArray;
//		m_TextureArray = 0;
//	}
//}

bool BUMPMAPPING::LoadModel(char* filename)
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
	model = new BUMPMAPModelType[vertexCount];
	if (!model)
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
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	// 모델 파일을 닫는다.
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


	// 모델의 면 수를 계산합니다.
	int faceCount = vertexCount / 3;

	// 모델 데이터에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 모든면을 살펴보고 접선, 비공식 및 법선 벡터를 계산합니다.
	for(int i=0; i<faceCount; i++)
	{
		// 모델에서이면에 대한 세 개의 정점을 가져옵니다.
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

		// 표면의 탄젠트와 바이 노멀을 계산합니다.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// 접선과 binormal을 사용하여 새 법선을 계산합니다.
		CalculateNormal(tangent, binormal, normal);

		// 모델 구조에서 면의 법선, 접선 및 바이 노멀을 저장합니다.
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


	// 현재 표면의 두 벡터를 계산합니다.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// 탄젠트 / 바이 노멀 방정식의 분모를 계산합니다.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// 법선을 표준화 한 다음 저장합니다.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// 이 법선의 길이를 계산합니다.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// 법선을 표준화 한 다음 저장합니다.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}


void BUMPMAPPING::CalculateNormal(BUMPMAPVectorType tangent, BUMPMAPVectorType binormal, BUMPMAPVectorType& normal)
{
	// 법선 벡터를 줄 수있는 접선과 binormal의 외적을 계산합니다.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// 법선의 길이를 계산합니다.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// 법선을 표준화합니다.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}