#pragma once

#include "d3dstdafx.h"


enum RotationDirection { X, Y, Z };


struct TextVertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

//===========================================

struct VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

typedef struct
{
	float x, y, z;
}VertexTypeF;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


//===================================

struct BUMPMAPVertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
};

struct BUMPMAPModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float tx, ty, tz;
	float bx, by, bz;
};

struct BUMPMAPTempVertexType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct BUMPMAPVectorType
{
	float x, y, z;
};

//===================================

struct LightBufferType
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float specularPower;
	D3DXVECTOR4 specularColor;
};