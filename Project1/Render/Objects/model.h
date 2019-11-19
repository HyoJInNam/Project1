#pragma once

struct TRANSFORM
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 orbitRot;
};


class LOADOBJECTSFILE;
class BUMPMAPPING;
class BumpMapShaderClass;
class COLORSHADER;
class LIGHT;
class LIGHTSHADER;

class MODEL
{
	TRANSFORM global;
	MODEL* parent;

public:
	MODEL();
	MODEL(const MODEL&);
	~MODEL();

	bool Initialize(char* modelFilename);
	void Shutdown();

	void SetTransformMatrix(RNDMATRIXS&);
	bool Render(RNDMATRIXS&, D3DXVECTOR3);

	void SetParent(MODEL*& parent) { this->parent = parent; }
	MODEL* GetParent() { return parent; }

	bool LoadTexture(WCHAR *);
	bool LoadTextures(WCHAR *, WCHAR *);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool Load(char*);

private:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	LOADOBJECTSFILE* file;
	COLORSHADER* colorShader;
	LIGHTSHADER* textureShader;

	BUMPMAPPING* bumpmap;
	BumpMapShaderClass* m_BumpMapShader;

//==============================

public:
	void SetTransformScale(float, float, float);
	void SetTransformPosition(float, float, float);
	void SetTransformRotation(float, float, float);
	void SetOrbitRot(float, float, float);

	void SetTransformScale(D3DXVECTOR3);
	void SetTransformPosition(D3DXVECTOR3);
	void SetTransformRotation(D3DXVECTOR3);

	D3DXVECTOR3 GetScale() { return global.scale; }
	D3DXVECTOR3 GetPosition() { return global.position; }
	D3DXVECTOR3 GetRotation() { return global.rotation; }
	D3DXVECTOR3 GetObitRotation() { return global.orbitRot; }

	void SetSpin(float, float, float);
	void SetOrbit(RNDMATRIXS&);
};







//==================================================

inline void MODEL::SetTransformScale(float x, float y, float z)
{
	global.scale.x = x;
	global.scale.y = y;
	global.scale.z = z;
}
inline void MODEL::SetTransformPosition(float x, float y, float z)
{
	global.position.x = x;
	global.position.y = y;
	global.position.z = z;
}
inline void MODEL::SetTransformRotation(float x, float y, float z)
{
	global.rotation.x = x;
	global.rotation.y = y;
	global.rotation.z = z;
}

inline void MODEL::SetTransformScale(D3DXVECTOR3 scl)
{
	SetTransformScale(scl.x, scl.y, scl.z);
}
inline void MODEL::SetTransformPosition(D3DXVECTOR3 pos)
{
	SetTransformPosition(pos.x, pos.y, pos.z);
}
inline void MODEL::SetTransformRotation(D3DXVECTOR3 rot)
{
	SetTransformRotation(rot.x, rot.y, rot.z);
}

inline void MODEL::SetSpin(float x, float y, float z)
{
	global.rotation.x += x;
	global.rotation.y += y;
	global.rotation.z += z;
}
inline void MODEL::SetOrbitRot(float x, float y, float z)
{
	global.orbitRot.x += x;
	global.orbitRot.y += y;
	global.orbitRot.z += z;
}
inline void MODEL::SetOrbit(RNDMATRIXS& renderMatrix)
{
	D3DXMATRIX Rx, Ry, Rz, T;

	if (!parent)
	{
		D3DXVECTOR3 pos = global.position;
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

		D3DXVECTOR3 orbitRot = global.orbitRot;
		D3DXMatrixRotationX(&Rx, orbitRot.x);
		D3DXMatrixRotationY(&Ry, orbitRot.y);
		D3DXMatrixRotationZ(&Rz, orbitRot.z);

		renderMatrix.world *= T * Rx * Ry * Rz;
		return;
	}

	D3DXVECTOR3 orbitRot = global.orbitRot;
	D3DXMatrixRotationX(&Rx, orbitRot.x);
	D3DXMatrixRotationY(&Ry, orbitRot.y);
	D3DXMatrixRotationZ(&Rz, orbitRot.z);

	D3DXVECTOR3 pos = global.position;
	D3DXVECTOR3 pPos = parent->GetPosition();
	D3DXVECTOR3 dis(((orbitRot.x != 0) ? (pPos.x - pos.x) : (pos.x - pPos.x))
				  , ((orbitRot.y != 0) ? (pPos.y - pos.y) : (pos.y - pPos.y))
				  , ((orbitRot.z != 0) ? (pPos.z - pos.z) : (pos.z - pPos.z)));
	D3DXMatrixTranslation(&T, dis.x, dis.y, dis.z);

	renderMatrix.world *= T * Rx * Ry * Rz;
	parent->SetOrbit(renderMatrix);
	return;
}


inline void MODEL::SetTransformMatrix(RNDMATRIXS& renderMatrix)
{
	D3DXMATRIX S, Rx, Ry, Rz, T;

	D3DXVECTOR3 scl = global.scale;
	D3DXMatrixScaling(&S, scl.x, scl.y, scl.z);

	D3DXVECTOR3 rot = global.rotation;
	D3DXMatrixRotationX(&Rx, rot.x);
	D3DXMatrixRotationY(&Ry, rot.y);
	D3DXMatrixRotationZ(&Rz, rot.z);

	renderMatrix.world = S * Rx * Ry * Rz;
	SetOrbit(renderMatrix);
}
