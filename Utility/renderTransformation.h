#pragma once


typedef struct RENDER_PIPELINE_MATRIXS
{
	D3DXMATRIX projection;
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX ortho;
	D3DXMATRIX lightView;
	D3DXMATRIX lightProjection;

}RNDMATRIXS;



typedef class RENDERTRANSFORMATION
{
public:
	RENDERTRANSFORMATION();
	~RENDERTRANSFORMATION();
	
	void Initialize();

	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix) { projectionMatrix = matrix.projection; }
	void GetWorldMatrix(D3DXMATRIX& worldMatrix) { worldMatrix = matrix.world; }
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix) { orthoMatrix = matrix.ortho; }
	void ResetViewport(ID3D11DeviceContext* deviceContext);

private:
	void CreateViewport();

private:
	FLOAT screenWidth, screenHeight;
	FLOAT screenNear, screenDepth;

protected:
	RNDMATRIXS matrix;
	D3D11_VIEWPORT viewport;

}RENDER_T;

