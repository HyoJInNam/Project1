#pragma once

class LOADFILE;
class LOADOBJECTSFILE;
class TextureArray;

class BUMPMAPPING: public LOADOBJECTSFILE
{
public:
	BUMPMAPPING(ID3D11Device*, ID3D11DeviceContext*);
	BUMPMAPPING(const BUMPMAPPING&);
	virtual ~BUMPMAPPING() override;

	bool Initialize(char* modelFilename, WCHAR* filename1, WCHAR* filename2);
	void Shutdown();
	void Render();

	//int GetIndexCount();
	bool LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2);
	ID3D11ShaderResourceView** GetTextures();

private:
	virtual bool InitializeBuffers() override;
	virtual void RenderBuffers() override;

	bool LoadModel(char*);
	void ReleaseModel();


	void CalculateModelVectors();
	void CalculateTangentBinormal(BUMPMAPTempVertexType, BUMPMAPTempVertexType, BUMPMAPTempVertexType, BUMPMAPVectorType&, BUMPMAPVectorType&);
	void CalculateNormal(BUMPMAPVectorType, BUMPMAPVectorType, BUMPMAPVectorType&);

private:
	BUMPMAPModelType* model;
	TextureArray* textures;
};