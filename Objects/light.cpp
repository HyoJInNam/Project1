
#include <objectstdafx.h>
#include "light.h"


LIGHT::LIGHT(const char* name)
	: Transform(name) 
{
	light = new LightBufferType;
}
LIGHT::LIGHT(const LIGHT & other): Transform(other) {}
LIGHT::~LIGHT() {}

bool LIGHT::ViewTransform()
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


			ImGui::Text("light");

			D3DXVECTOR3& direction = light->lightDirection;
			ImGui::DragFloat3("direction", (float*)&direction, 0.1f, 0, 0);

			//D3DXVECTOR4 ambient = obj_light->GetAmbientColor();
			//ImGui::DragFloat4("ambient", (float*)&ambient, 0.1f, 0, 0);
			//obj_light->SetAmbientColor(ambient);

			//D3DXVECTOR4 diffuse = obj_light->GetDiffuseColor();
			//ImGui::DragFloat4("diffuse", (float*)&diffuse, 0.1f, 0, 0);
			//obj_light->SetDiffuseColor(diffuse);

			//D3DXVECTOR4 specular = obj_light->GetSpecularColor();
			//ImGui::DragFloat4("specular", (float*)&specular, 0.1f, 0, 0);
			//obj_light->SetSpecularColor(specular);

			//float specularPower = obj_light->GetSpecularPower();
			//ImGui::DragFloat("specular power", (float*)&specularPower, 0.1f, 0, 0);
			//obj_light->SetSpecularPower(specularPower);
			//SetLIGHT(obj_light);
		}
		ImGui::End();
	}
	return true;
}

void LIGHT::SetDirectionLight()
{
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(10.0f);
}

void LIGHT::SetPointLight()
{
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDirection(0.0f, 0.0f, 1.0f);
	//---------------------------------------
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(10.0f);
	//---------------------------------------
}

void LIGHT::SetAmbientColor(float red, float green, float blue, float alpha) { light->ambientColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetAmbientColor(D3DXVECTOR4 light){ SetAmbientColor(light.x, light.y, light.z, light.w); }
void LIGHT::SetDiffuseColor(float red, float green, float blue, float alpha) { light->diffuseColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetDiffuseColor(D3DXVECTOR4 light){ SetDiffuseColor(light.x, light.y, light.z, light.w); }
void LIGHT::SetDirection(float x, float y, float z) { light->lightDirection = D3DXVECTOR3(x, y, z); }
void LIGHT::SetDirection(D3DXVECTOR3 light) { SetDirection(light.x, light.y, light.z); }
void LIGHT::SetSpecularColor(float red, float green, float blue, float alpha) { light->specularColor = D3DXVECTOR4(red, green, blue, alpha); }
void LIGHT::SetSpecularColor(D3DXVECTOR4 light){ SetSpecularColor(light.x, light.y, light.z, light.w); }
void LIGHT::SetSpecularPower(float power) { light->specularPower = power; }


D3DXVECTOR4 LIGHT::GetAmbientColor() { return light->ambientColor; }
D3DXVECTOR4 LIGHT::GetDiffuseColor() { return light->diffuseColor; }
D3DXVECTOR3 LIGHT::GetDirection() { return light->lightDirection; }
D3DXVECTOR4 LIGHT::GetSpecularColor() { return light->specularColor; }
float LIGHT::GetSpecularPower() { return light->specularPower; }