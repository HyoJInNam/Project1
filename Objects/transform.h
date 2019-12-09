#pragma once


#include "objectstdafx.h"


template<typename T>
class Transform
{
protected:
	HWND hwnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	const char* name;
	bool show_inspector;
	 
public:
	const char* GetObjectName() { return name; }
	void SetGuiWindow(bool show_window) { this->show_inspector = show_window; }
	bool GetGuiWindow() { return show_inspector; }

	void ShowObjectInspector();
	virtual bool ViewTransform();



protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 orbitRot;

public:
	Transform(const char* name);
	Transform(const Transform& other) {};
	~Transform() {};

	void SetScale(float, float, float);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetOrbitRot(float, float, float);

	void SetScale(D3DXVECTOR3);
	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);

	D3DXVECTOR3 GetScale() { return scale; }
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetRotation() { return rotation; }
	D3DXVECTOR3 GetObitRotation() { return orbitRot; }

	void GetPosition(float& x, float& y, float& z) { x = position.x; y = position.y; z = position.z;}
	void GetPosition(D3DXVECTOR3 pos) { pos = position; }

	void SetSpin(float, float, float);
	void SetOrbit(RNDMATRIXS*);







protected:
	Transform* parent;

public:
	void SetTransformMatrix(RNDMATRIXS*);
	void SetParent(Transform* parent) { this->parent = parent; }
	Transform* GetParent() { return parent; }

};




template<typename T>
Transform<T>::Transform(const char* name)
	: name(name), show_inspector(false)
	, position(0.0f, 0.0f, 0.0f)
	, rotation(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, orbitRot(0.0f, 0.0f, 0.0f)
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();
}


template<typename T>
void Transform<T>::ShowObjectInspector()
{
	ImGui::Checkbox(name, &show_inspector);
}

template<typename T>
bool Transform<T>::ViewTransform()
{
	if (show_inspector)
	{
		ImGui::Begin(GetObjectName(), &show_inspector);
		{
			ImGui::Text("transform");
			D3DXVECTOR3 pos = GetPosition();
			ImGui::DragFloat3("position", (float*)&pos, 0.1f, 0, 0);
			SetPosition(pos);

			D3DXVECTOR3 rot = GetRotation();
			ImGui::DragFloat3("rotation", (float*)&rot, 0.1f, 0, 0);
			SetRotation(rot);

			D3DXVECTOR3 scl = GetScale();
			ImGui::DragFloat3("scale", (float*)&scl, 0.1f, 0, 0);
			SetScale(scl);

		}
		ImGui::End();
	}
	return true;
}




template<typename T>
inline void Transform<T>::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

template<typename T>
inline void Transform<T>::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

template<typename T>
inline void Transform<T>::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}


template<typename T>
inline void Transform<T>::SetScale(D3DXVECTOR3 scl)
{
	SetScale(scl.x, scl.y, scl.z);
}

template<typename T>
inline void Transform<T>::SetPosition(D3DXVECTOR3 pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

template<typename T>
inline void Transform<T>::SetRotation(D3DXVECTOR3 rot)
{
	SetRotation(rot.x, rot.y, rot.z);
}


template<typename T>
inline void Transform<T>::SetSpin(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

template<typename T>
inline void Transform<T>::SetOrbitRot(float x, float y, float z)
{
	orbitRot.x += x;
	orbitRot.y += y;
	orbitRot.z += z;
}

template<typename T>
inline void Transform<T>::SetOrbit(RNDMATRIXS* renderMatrix)
{
	D3DXMATRIX Rx, Ry, Rz, P;

	if (!parent)
	{
		D3DXMatrixTranslation(&P, position.x, position.y, position.z);

		D3DXMatrixRotationX(&Rx, orbitRot.x);
		D3DXMatrixRotationY(&Ry, orbitRot.y);
		D3DXMatrixRotationZ(&Rz, orbitRot.z);

		renderMatrix->world *= P * Rx * Ry * Rz;
		return;
	}

	D3DXMatrixRotationX(&Rx, orbitRot.x);
	D3DXMatrixRotationY(&Ry, orbitRot.y);
	D3DXMatrixRotationZ(&Rz, orbitRot.z);

	D3DXVECTOR3 pPos = parent->GetPosition();
	D3DXVECTOR3 dis(((orbitRot.x != 0) ? (pPos.x - position.x) : (position.x - pPos.x))
		, ((orbitRot.y != 0) ? (pPos.y - position.y) : (position.y - pPos.y))
		, ((orbitRot.z != 0) ? (pPos.z - position.z) : (position.z - pPos.z)));
	D3DXMatrixTranslation(&P, dis.x, dis.y, dis.z);

	renderMatrix->world *= P * Rx * Ry * Rz;
	parent->SetOrbit(renderMatrix);
	return;
}



template<typename T>
inline void Transform<T>::SetTransformMatrix(RNDMATRIXS* renderMatrix)
{
	D3DXMATRIX S, Rx, Ry, Rz;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

	D3DXMatrixRotationX(&Rx, rotation.x);
	D3DXMatrixRotationY(&Ry, rotation.y);
	D3DXMatrixRotationZ(&Rz, rotation.z);

	renderMatrix->world = S * Rx * Ry * Rz;
	SetOrbit(renderMatrix);
}
