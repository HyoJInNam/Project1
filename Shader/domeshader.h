#pragma once
class SHADER;

class DOMESHADER : public AlignedAllocationPolicy<16>, SHADER
{
public:
	DOMESHADER(
		HWND hwnd,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext);
	DOMESHADER(const DOMESHADER&);
	virtual ~DOMESHADER()override;

	virtual bool Initialize() override;
	bool Render(
		int indexCount,
		RNDMATRIXS& matrixs,
		GradientBufferType& color);

private:
	virtual bool InitializeShader(WCHAR*, WCHAR*) override;
	virtual bool InitializeShaderBuffer() override;
	virtual void ShutdownShader()override;

	bool SetShaderParameters(
		RNDMATRIXS& matrixs,
		GradientBufferType& color);

private:
	ID3D11Buffer* gradientBuffer = nullptr;
};