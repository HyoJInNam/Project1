#pragma once


class LIGHT : public Transform <LIGHT>
{
public:
	LIGHT(const char* name);
	LIGHT(const LIGHT& other);
	~LIGHT();

	virtual bool ViewTransform() override;
	//======================================================

	LightBufferType* GetLight() { return light; }
	void SetDirectionLight();
	void SetPointLight();
	void SetLookAt(float x, float y, float z)
	{
		m_lookAt.x = x;
		m_lookAt.y = y;
		m_lookAt.z = z;
		return;
	}

	//======================================================

	void SetAmbientColor(float, float, float, float);
	void SetAmbientColor(D3DXVECTOR4);
	void SetDiffuseColor(float, float, float, float);
	void SetDiffuseColor(D3DXVECTOR4);
	void SetDirection(float, float, float);
	void SetDirection(D3DXVECTOR3);
	void SetDirection();
	void SetSpecularColor(float, float, float, float);
	void SetSpecularColor(D3DXVECTOR4);
	void SetSpecularPower(float);


	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor(); 
	float GetSpecularPower();
	//======================================================
	void GenerateViewMatrix(); 
	void GetViewMatrix(D3DXMATRIX&); 

	void GenerateOrthoMatrix(float, float);
	void GetOrthoMatrix(D3DXMATRIX&);


protected:
	LightBufferType* light;

	D3DXVECTOR3 m_lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_orthMatrix;
};