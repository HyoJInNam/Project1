#pragma once

class MODEL;

class LOADOBJECTSFILE
{
	friend class MODEL;
private:
	LOADOBJECTSFILE(); 
	LOADOBJECTSFILE(const LOADOBJECTSFILE&);
	~LOADOBJECTSFILE();

public:
	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadObject(char* filename);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);

private:
	ModelType* object;
	int m_vertexCount, m_indexCount;
};

