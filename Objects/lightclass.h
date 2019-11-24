#pragma once

class LIGHT
{
public:
	LIGHT();
	~LIGHT();

	//======================================================

	LightBufferType* GetLight() { return light; }
	void SetDirectionLight();
	void SetPointLight();

	//======================================================

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetDirection(D3DXVECTOR3);
	void SetSpecularColor(float, float, float, float);
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