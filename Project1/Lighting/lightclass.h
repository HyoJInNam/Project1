#pragma once

class LIGHT
{
public:
	LIGHT();
	~LIGHT();

	//======================================================

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

	void SetAtt(D3DXVECTOR3);
	void SetPos(D3DXVECTOR3);
	void Setrange(float);

	D3DXVECTOR3 GetAtt();
	D3DXVECTOR3 GetPos();
	float Getrange();
	//======================================================

	void SetLight(LightBufferType& light) { *this->light = light; }
	LightBufferType* GetLight() { return light;	}
	void SetLight(LIGHT_TYPE lType) { this->light->lightType = lType; }

protected:
	LightBufferType* light;
};