#pragma once
class SHADER;

class DepthShaderClass : public SHADER
{

public:
	DepthShaderClass(
		HWND hwnd,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext);
	DepthShaderClass(const DepthShaderClass&);
	virtual ~DepthShaderClass()override;

	virtual bool Initialize() override;
	bool Render(
		int indexCount,
		RNDMATRIXS& matrixs);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;
	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(RNDMATRIXS matrixs);

};