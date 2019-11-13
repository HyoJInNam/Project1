#include "../Utility/renafx.h"
#include "lightclass.h"


LIGHT::LIGHT(){ light = new LightBufferType; }
LIGHT::~LIGHT() {}

void LIGHT::SetDirectionLight()
{
	light->lightType = LIGHT_DIRECTION;
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(10.0f);
}

void LIGHT::SetPointLight()
{
	light->lightType = LIGHT_POINTLIGHT;
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(70.0f);
	//---------------------------------------
}

void LIGHT::SetAmbientColor(float red, float green, float blue, float alpha) { light->directLight.ambientColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetDiffuseColor(float red, float green, float blue, float alpha) { light->directLight.diffuseColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetDirection(float x, float y, float z) { light->directLight.lightDirection = D3DXVECTOR3(x, y, z); }
void LIGHT::SetDirection(D3DXVECTOR3 light) { SetDirection(light.x, light.y, light.z); }
void LIGHT::SetSpecularColor(float red, float green, float blue, float alpha) { light->directLight.specularColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetSpecularPower(float power) { light->directLight.specularPower = power; }


D3DXVECTOR4 LIGHT::GetAmbientColor() { return light->directLight.ambientColor; }
D3DXVECTOR4 LIGHT::GetDiffuseColor() { return light->directLight.diffuseColor; }
D3DXVECTOR3 LIGHT::GetDirection() { return light->directLight.lightDirection; }
D3DXVECTOR4 LIGHT::GetSpecularColor() { return light->directLight.specularColor; }
float LIGHT::GetSpecularPower() { return light->directLight.specularPower; }

void LIGHT::SetAtt(D3DXVECTOR3 att) { light->pointLight.att = att; }
void LIGHT::SetPos(D3DXVECTOR3 pos){ light->pointLight.pos = pos; }
void LIGHT::Setrange(float range){ light->pointLight.range = range; }

D3DXVECTOR3 LIGHT::GetAtt(){	return light->pointLight.att; }
D3DXVECTOR3 LIGHT::GetPos() { return light->pointLight.pos; }
float LIGHT::Getrange(){	return light->pointLight.range; }
