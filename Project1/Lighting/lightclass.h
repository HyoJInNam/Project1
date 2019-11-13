#pragma once

class LIGHT
{
public:
	LIGHT();
	LIGHT(const LIGHT&);
	~LIGHT();

	//======================================================

	void SetLight();
	void SetDirectionLight();
	void SetPointLight(D3DXVECTOR3 object);

	//======================================================

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetDirection(D3DXVECTOR3);

	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	//======================================================

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

	D3DXVECTOR4 GetSpecularColor();
	float GetSpecularPower();

	//======================================================


	//void SetAtt(float, float, float);
	//void SetPos(float, float, float);
	//void SetPos(D3DXVECTOR3);
	//void SetRange(float);


	//D3DXVECTOR3 GetAtt();
	//D3DXVECTOR3 GetPos();
	//float GetRange();


	LightBufferType GetLight() { return *light;	}
private:
	LightBufferType* light;
};