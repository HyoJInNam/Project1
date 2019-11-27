#pragma once

class LOADFILE;

class LOADOBJECTSFILE : public LOADFILE
{
	ModelType* object;
public:
	LOADOBJECTSFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADOBJECTSFILE(const LOADOBJECTSFILE&);
	virtual ~LOADOBJECTSFILE() override;


	bool Initialize(char* modelFilename);
	void Render();
	void Shutdown();


	bool LoadFile_txt(char*);
	bool LoadFile_obj(char*);


private:
	virtual bool InitializeBuffers() override;
	virtual void RenderBuffers() override;
	void ReleaseModel();


	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);



	//Bump mapping calculate
private:
	void CalculateModelVectors();
	void CalculateTangentBinormal(ModelType, ModelType, ModelType, VertexTypeF&, VertexTypeF&);
	void CalculateNormal(VertexTypeF, VertexTypeF, VertexTypeF&);
};