#pragma once

class LOADOBJECTSFILE
{

public:
	LOADOBJECTSFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADOBJECTSFILE(const LOADOBJECTSFILE&);
	~LOADOBJECTSFILE();

	bool InitializeBuffers();
	void RenderBuffers();
	void ShutdownBuffers();

	bool LoadTextFile(char*);
	bool LoadObjFile(char*);
	void ReleaseModel();

	int GetIndexCount() { return indexCount; }

private:
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ModelType* object;
	ID3D11Buffer* vertexBuffer, * indexBuffer;
	int vertexCount, indexCount;
};

