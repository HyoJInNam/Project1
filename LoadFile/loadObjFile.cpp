#include "LoadFile.h"
#include "loadObjFile.h"

LOADOBJECTSFILE::LOADOBJECTSFILE(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:LOADFILE(device, deviceContext)
{}

LOADOBJECTSFILE::LOADOBJECTSFILE(const LOADOBJECTSFILE&) : LOADFILE(this) {};
LOADOBJECTSFILE::~LOADOBJECTSFILE()
{
	ReleaseModel();
}

bool LOADOBJECTSFILE::Initialize(char* modelFilename)
{
	CString m_temp = modelFilename;
	CString temp = m_temp.Right(4);
	if (temp.CompareNoCase(_T(".obj")) == 0) { ISFAIL(LoadFile_obj(modelFilename)); }
	if (temp.CompareNoCase(_T(".txt")) == 0) { ISFAIL(LoadFile_txt(modelFilename)); }
	
	CalculateModelVectors();
	InitializeBuffers();
	return true;
}

void LOADOBJECTSFILE::Render()
{
	RenderBuffers();
}

void LOADOBJECTSFILE::Shutdown()
{
	ShutdownBuffers();
	ReleaseModel();
	ReleaseTexture();
}



bool LOADOBJECTSFILE::InitializeBuffers()
{
	VertexType* vertices = new VertexType[vertexCount];
	ISINSTANCE(vertices);

	// Create the index array.
	unsigned long* indices = new unsigned long[indexCount];
	ISINSTANCE(indices);

	// 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(object[i].x, object[i].y, object[i].z);
		vertices[i].texture = D3DXVECTOR2(object[i].tu, object[i].tv);
		vertices[i].normal = D3DXVECTOR3(object[i].nx, object[i].ny, object[i].nz);
		vertices[i].tangent = D3DXVECTOR3(object[i].tx, object[i].ty, object[i].tz);
		vertices[i].binormal = D3DXVECTOR3(object[i].bx, object[i].by, object[i].bz);

		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
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
void LOADOBJECTSFILE::RenderBuffers()
{
	unsigned int  stride = sizeof(VertexType);
	unsigned int  offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
void LOADOBJECTSFILE::ReleaseModel()
{
	vertexCount = 0;
	indexCount = 0;
	SAFE_DELETE_ARRAY(object);
	return;
}


bool LOADOBJECTSFILE::LoadFile_txt(char* filename)
{
	ifstream fin;
	char input;


	fin.open(filename);
	if (fin.fail()) { ISFAIL(LoadFile_obj(filename)); }

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	object = new ModelType[vertexCount];
	ISINSTANCE(object);

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i < vertexCount; i++)
	{
		fin >> object[i].x >> object[i].y >> object[i].z;
		fin >> object[i].tu >> object[i].tv;
		fin >> object[i].nx >> object[i].ny >> object[i].nz;
	}

	fin.close();

	return true;
}
bool LOADOBJECTSFILE::LoadFile_obj(char* filename)
{
	int vertexCount, textureCount, normalCount, faceCount;

	ISFAIL(ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount));
	ISFAIL(LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount));
	
	CString m_temp = filename;
	m_temp.Replace(m_temp.Right(4), _T(".txt"));

	USES_CONVERSION;
	char* modelName = W2A(m_temp);
	LoadFile_txt(modelName);
	return true;

}
bool LOADOBJECTSFILE::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	fin.open(filename);

	if (fin.fail() == true)
	{
		return false;
	}

	fin.get(input);

	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}
		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}
		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Start reading the beginning of the next line.
		fin.get(input);
	}

	fin.close();
	return true;
}
bool LOADOBJECTSFILE::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexTypeF* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex = 0, texcoordIndex = 0, normalIndex = 0, faceIndex = 0;
	int vIndex, tIndex, nIndex;
	char input = NULL, input2 = NULL;
	ofstream fout;


	vertices = new VertexTypeF[vertexCount];
	ISINSTANCE(vertices);
	texcoords = new VertexTypeF[textureCount];
	ISINSTANCE(texcoords);
	normals = new VertexTypeF[normalCount];
	ISINSTANCE(normals);
	faces = new FaceType[faceCount];
	ISINSTANCE(faces);


	fin.open(filename);

	if (fin.fail() == true)
	{
		return false;
	}
	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z; 
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y; 
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}
			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z; 
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}
		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2
					>> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}
		// Read in the remainder of the line.
		while (input != '\n') { fin.get(input); }

		// Start reading the beginning of the next line.
		fin.get(input);
	}


	CString m_temp = filename;
	m_temp.Replace(m_temp.Right(4), _T(".txt"));

	USES_CONVERSION;
	char* modelName = W2A(m_temp);
	fin.close();

	fout.open(modelName);

	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	fout.close();

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(texcoords);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(faces);
	return true;
}


void LOADOBJECTSFILE::CalculateModelVectors()
{
	ModelType vertex1, vertex2, vertex3;
	VertexTypeF tangent, binormal, normal;


	// 모델의 면 수를 계산합니다.
	int faceCount = vertexCount / 3;

	// 모델 데이터에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 모든면을 살펴보고 접선, 비공식 및 법선 벡터를 계산합니다.
	for (int i = 0; i < faceCount; i++)
	{
		// 모델에서이면에 대한 세 개의 정점을 가져옵니다.
		vertex1.x = object[index].x;
		vertex1.y = object[index].y;
		vertex1.z = object[index].z;
		vertex1.tu = object[index].tu;
		vertex1.tv = object[index].tv;
		vertex1.nx = object[index].nx;
		vertex1.ny = object[index].ny;
		vertex1.nz = object[index].nz;
		index++;

		vertex2.x = object[index].x;
		vertex2.y = object[index].y;
		vertex2.z = object[index].z;
		vertex2.tu = object[index].tu;
		vertex2.tv = object[index].tv;
		vertex2.nx = object[index].nx;
		vertex2.ny = object[index].ny;
		vertex2.nz = object[index].nz;
		index++;

		vertex3.x = object[index].x;
		vertex3.y = object[index].y;
		vertex3.z = object[index].z;
		vertex3.tu = object[index].tu;
		vertex3.tv = object[index].tv;
		vertex3.nx = object[index].nx;
		vertex3.ny = object[index].ny;
		vertex3.nz = object[index].nz;
		index++;

		// 표면의 탄젠트와 바이 노멀을 계산합니다.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// 접선과 binormal을 사용하여 새 법선을 계산합니다.
		CalculateNormal(tangent, binormal, normal);

		// 모델 구조에서 면의 법선, 접선 및 바이 노멀을 저장합니다.
		object[index - 1].nx = normal.x;
		object[index - 1].ny = normal.y;
		object[index - 1].nz = normal.z; 
		object[index - 2].nx = normal.x;
		object[index - 2].ny = normal.y;
		object[index - 2].nz = normal.z;
		object[index - 2].tx = tangent.x;
		object[index - 2].ty = tangent.y;
		object[index - 2].tz = tangent.z;
		object[index - 2].bx = binormal.x;
		object[index - 2].by = binormal.y;
		object[index - 2].bz = binormal.z;

		object[index - 3].nx = normal.x;
		object[index - 3].ny = normal.y;
		object[index - 3].nz = normal.z;
		object[index - 3].tx = tangent.x;
		object[index - 3].ty = tangent.y;
		object[index - 3].tz = tangent.z;
		object[index - 3].bx = binormal.x;
		object[index - 3].by = binormal.y;
		object[index - 3].bz = binormal.z;
	}
}
void LOADOBJECTSFILE::CalculateTangentBinormal(ModelType vertex1, ModelType vertex2, ModelType vertex3,	VertexTypeF& tangent, VertexTypeF& binormal)
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
void LOADOBJECTSFILE::CalculateNormal(VertexTypeF tangent, VertexTypeF binormal, VertexTypeF& normal)
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
