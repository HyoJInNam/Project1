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

	//======================================================

	void SetAmbientColor(float, float, float, float);
	void SetAmbientColor(D3DXVECTOR4);
	void SetDiffuseColor(float, float, float, float);
	void SetDiffuseColor(D3DXVECTOR4);
	void SetDirection(float, float, float);
	void SetDirection(D3DXVECTOR3);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularColor(D3DXVECTOR4);
	void SetSpecularPower(float);


	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor(); 
	float GetSpecularPower();
	//======================================================


protected:
	LightBufferType* light;
};