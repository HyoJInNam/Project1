#pragma once

class LOADFILE;

class LOADDOMEFILE : public LOADFILE
{

	ModelType* object;
public:
	LOADDOMEFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADDOMEFILE(const LOADDOMEFILE&);
	virtual ~LOADDOMEFILE() override;

	bool Initialize(char* modelFilename);
	void Render();
	void Shutdown();

	bool LoadFile_txt(char*);

private:
	virtual bool InitializeBuffers() override;
	virtual void RenderBuffers() override;

	void ReleaseObject();
};

