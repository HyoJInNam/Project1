#pragma once


class SHADER;

class COLORSHADER: public SHADER
{
	ID3D11Buffer* matrixBuffer;

public:
	COLORSHADER(HWND, ID3D11Device*, ID3D11DeviceContext*);
	COLORSHADER(const COLORSHADER&);
	virtual ~COLORSHADER() override;

	virtual bool Initialize() override;
	bool Render(int, RNDMATRIXS);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;
	bool SetShaderParameters();

};
