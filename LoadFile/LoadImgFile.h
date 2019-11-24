#pragma once

class LOADFILE;

class LOADIMGFILE : public LOADFILE
{
public:
	LOADIMGFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADIMGFILE(const LOADIMGFILE&);
	~LOADIMGFILE();

	virtual bool InitializeBuffers() override;
	virtual void RenderBuffers() override;
	bool UpdateBuffers(float left, float right, float top, float bottom);

private:

};