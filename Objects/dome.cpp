
#include <objectstdafx.h>
#include "dome.h"

#include <fstream>
using namespace std;


DOME::DOME(const char* name) :
	Transform(name),
	color{
	D3DXVECTOR4(0.3f, 0.6f, 0.8f, 1.0f),
	D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
	D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f)}
{
	file = new LOADDOMEFILE(device, deviceContext);
}
DOME::DOME(const DOME& other) : Transform(other) {}
DOME::~DOME() {}


bool DOME::Initialize(char* modelFilename)
{
	ISFAIL(file->Initialize(modelFilename));
	//if(!LoadSkyDomeModel("../Dx11Terrain_10/data/skydome.txt"))


	skyDomeShader = new DOMESHADER(hwnd, device, deviceContext);
	ISINSTANCE(skyDomeShader);
	if (!skyDomeShader->Initialize())
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void DOME::Shutdown()
{
	SAFE_DELETE(skyDomeShader);
	file->Shutdown();
}


void DOME::Render(D3D* d3d, RNDMATRIXS& renderMatrixs)
{
	d3d->TurnOffCulling();
	d3d->TurnZBufferOff();

	file->Render();
	ViewTransform();
	skyDomeShader->Render(file->GetIndexCount(), renderMatrixs, color);


	d3d->TurnOnCulling();
	d3d->TurnZBufferOn();

}

bool DOME::ViewTransform()
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

			ImGui::Text("color");
			ImGui::DragFloat3("topColor", (float*)&color.topColor, 0.1f, 0, 0);
			ImGui::DragFloat3("centerColor", (float*)&color.centerColor, 0.1f, 0, 0);
			ImGui::InputFloat4("bottomColor", (float*)&color.bottomColor);

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
