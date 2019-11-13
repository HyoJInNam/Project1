#include "../Utility/renafx.h"
#include "lightclass.h"


LIGHT::LIGHT():light(nullptr)
{
	light = new LightBufferType;
}


LIGHT::LIGHT(const LIGHT& other) {}
LIGHT::~LIGHT() {}

void LIGHT::SetDirectionLight()
{
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(10.0f);
}

void LIGHT::SetPointLight(D3DXVECTOR3 object)
{
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(70.0f);
	////---------------------------------------
}

void LIGHT::SetAmbientColor(float red, float green, float blue, float alpha)
{
	light->ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LIGHT::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	light->diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}


void LIGHT::SetDirection(float x, float y, float z)
{
	light->lightDirection = D3DXVECTOR3(x, y, z);
}

void LIGHT::SetDirection(D3DXVECTOR3 light)
{
	SetDirection(light.x, light.y, light.z);
}

void LIGHT::SetSpecularColor(float red, float green, float blue, float alpha)
{
	light->specularColor = D3DXVECTOR4(red, green, blue, alpha);
}


void LIGHT::SetSpecularPower(float power)
{
	light->specularPower = power;
}
D3DXVECTOR4 LIGHT::GetAmbientColor()
{
	return light->ambientColor;
}

D3DXVECTOR4 LIGHT::GetDiffuseColor()
{
	return light->diffuseColor;
}


D3DXVECTOR3 LIGHT::GetDirection()
{
	return light->lightDirection;
}

D3DXVECTOR4 LIGHT::GetSpecularColor()
{
	return light->specularColor;
}


float LIGHT::GetSpecularPower()
{
	return light->specularPower;
}
