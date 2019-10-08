#include "renafx.h"
#include "model.h"



MODEL::MODEL()
	:m_vertexBuffer(0), m_indexBuffer(0)
{
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();
}


MODEL::MODEL(const MODEL& other) {}
MODEL::~MODEL() {}


bool MODEL::Initialize(char* modelFilename)
{
	CString m_temp = modelFilename;
	CString temp = m_temp.Right(4);
	if (temp.CompareNoCase(_T(".txt")) == 0) { ISFAIL(LoadModel(modelFilename)); }
	//else if	(temp.CompareNoCase(_T(".obj")) == 0) { ISFAIL(LoadObject(modelFilename)); }
	ISFAIL(InitializeBuffers());

	return true;
}


void MODEL::Shutdown()
{
	ShutdownBuffers();
	ReleaseModel();

	return;
}


void MODEL::Render()
{
	RenderBuffers();

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
	vertices = new VertexType[m_vertexCount];
	ISINSTANCE(vertices);

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	ISINSTANCE(indices);

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = D3DXVECTOR2(model[i].tu, model[i].tv);
		vertices[i].normal = D3DXVECTOR3(model[i].nx, model[i].ny, model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
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


bool MODEL::LoadModel(char* filename)
{
	ifstream fin;
	char input;


	fin.open(filename);
	if (fin.fail()) { ISFAIL(LoadObject(filename)); }

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	model = new ModelType[m_vertexCount];
	ISINSTANCE(model);

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	fin.close();

	return true;
}
void MODEL::ReleaseModel()
{
	SAFE_DELETE_ARRAY(model);
	return;
}


bool MODEL::LoadObject(char* filename)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	ISFAIL(result);

	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	ISFAIL(result);

	return true;

}

bool MODEL::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
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
bool MODEL::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexTypeF *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
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

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

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
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}
			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;
				// Invert the Z normal to change to left hand system.
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


