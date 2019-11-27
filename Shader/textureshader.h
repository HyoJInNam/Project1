#pragma once


class SHADER;

class TEXTURESHADER:public SHADER
{
	ID3D11Buffer* matrixBuffer;

public:
	TEXTURESHADER(HWND, ID3D11Device*, ID3D11DeviceContext*);
	TEXTURESHADER(const TEXTURESHADER&);
	virtual~TEXTURESHADER() override;

	virtual bool Initialize() override;
	bool Render(int, RNDMATRIXS, ID3D11ShaderResourceView* texture);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;

	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;
	bool SetShaderParameters(ID3D11ShaderResourceView* texture);

};
