#pragma once


typedef struct RENDER_PIPELINE_MATRIXS
{
	D3DXMATRIX projection;
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX ortho;

}RNDMATRIXS;



typedef class RENDERTRANSFORMATION
{
public:
	RENDERTRANSFORMATION();
	~RENDERTRANSFORMATION();
	
	void Initialize();

	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
	{
		projectionMatrix = matrix.projection;
		return;
	}
	void GetWorldMatrix(D3DXMATRIX& worldMatrix)
	{
		worldMatrix = matrix.world;
		return;
	}
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
	{
		orthoMatrix = matrix.ortho;
		return;
	}

private:
	void CreateViewport();

private:
	FLOAT screenWidth, screenHeight;
	FLOAT screenNear, screenDepth;

protected:
	RNDMATRIXS matrix;

}RENDER_T;

