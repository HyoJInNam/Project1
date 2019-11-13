#pragma once



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

struct DirectionLightBuffer
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float specularPower;
	D3DXVECTOR4 specularColor;
};

struct PointLightBufferType
{
	D3DXVECTOR3 att;
	D3DXVECTOR3 pos;
	float range;
};


//===================================

enum LIGHT_TYPE {
	LIGHT_NONE,
	LIGHT_DIRECTION,
	LIGHT_POINTLIGHT
};

struct LightBufferType
{
	DirectionLightBuffer directLight;
	PointLightBufferType pointLight;
	LIGHT_TYPE lightType = LIGHT_NONE;
};