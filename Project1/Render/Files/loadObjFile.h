#pragma once

//#include "./LoadFile.h"

class LOADFILE;

class LOADOBJECTSFILE : public LOADFILE
{
public:
	LOADOBJECTSFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADOBJECTSFILE(const LOADOBJECTSFILE&);
	virtual ~LOADOBJECTSFILE() override;

	virtual bool InitializeBuffers() override;
	virtual void RenderBuffers() override;

	bool LoadTextFile(char*);
	bool LoadObjFile(char*);
	void ReleaseModel();

private:
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);

private:
	ModelType* object;

};