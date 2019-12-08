
#include "objectstdafx.h"
#include "light.h"
#include "model.h"



MODEL::MODEL(const char* name) : Transform(name)
{
	file = new LOADOBJECTSFILE(device, deviceContext);
}


MODEL::MODEL(const MODEL& other) : Transform(other) {}
MODEL::~MODEL() {}

bool MODEL::ViewTransform()
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

			ImGui::Text("light");
			D3DXVECTOR4 ambient = light->GetAmbientColor();
			ImGui::DragFloat4("ambient", (float*)&ambient, 0.1f, 0, 0);
			light->SetAmbientColor(ambient);

			D3DXVECTOR4 diffuse = light->GetDiffuseColor();
			ImGui::DragFloat4("diffuse", (float*)&diffuse, 0.1f, 0, 0);
			light->SetDiffuseColor(diffuse);

			D3DXVECTOR4 specular = light->GetSpecularColor();
			ImGui::DragFloat4("specular", (float*)&specular, 0.1f, 0, 0);
			light->SetSpecularColor(specular);

			float specularPower = light->GetSpecularPower();
			ImGui::DragFloat("specular power", (float*)&specularPower, 0.1f, 0, 0);
			light->SetSpecularPower(specularPower);
		}
		ImGui::End();
	}
	return true;
}

bool MODEL::Initialize(char* modelFilename, WCHAR* texture_file_name, WCHAR* normal_texture_name, WCHAR* specularmap)
{
	ISFAIL(file->Initialize(modelFilename));
	file->LoadTexture(device, texture_file_name, normal_texture_name, specularmap);

	light = new LIGHT("obj shader");
	ISINSTANCE(light);
	light->SetDirectionLight();
	if (!light->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light object.", L"ERROR");
		return false;
	}

	if (specularmap)
	{
		m_SpecMapShader = new SpecMapShaderClass(hwnd, device, deviceContext);
		ISINSTANCE(m_SpecMapShader);
		if (IsInk == true)
		{
			if (!m_SpecMapShader->InkInitialize())
			{
				MessageBox(hwnd, L"Could not initialize the ink shader object.", L"Error", MB_OK);
				return false;
			}
		}
		else
		{
			if (!m_SpecMapShader->Initialize())
			{
				MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
				return false;
			}
		}

	}
	else if (normal_texture_name)
	{
		m_BumpMapShader = new BumpMapShaderClass(hwnd, device, deviceContext);
		ISINSTANCE(m_BumpMapShader);
		if (!m_BumpMapShader->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the Bump Map shader object.", L"ERROR");
			return false;
		}
	}
	else {
		if (IsInk == true)
		{
			toonShader = new LIGHTSHADER(hwnd, device, deviceContext);
			ISINSTANCE(toonShader);
			if (!toonShader->ToonInitialize())
			{
				ERR_MESSAGE(L"Could not initialize the toon shader object.", L"ERROR");
				return false;
			}
		}
	}


	m_DepthShader = new DepthShaderClass(hwnd, device, deviceContext);
	ISINSTANCE(m_DepthShader);
	if (!m_DepthShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the depth shader object.", L"ERROR");
		return false;
	}


	m_ShadowShader = new ShadowShaderClass(hwnd, device, deviceContext);
	ISINSTANCE(m_ShadowShader);
	if (!m_ShadowShader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the  shadow shader object.", L"ERROR");
		return false;
	}

	
	
	return true;
}

bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos, LIGHT* mainLight)
{
	SetTransformMatrix(&renderMatrix);
	file->Render();
	light->SetDirection(mainLight->GetDirection());


	if (m_SpecMapShader)
	{
		m_SpecMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
		//return ViewTransform();
	}
	else if (m_BumpMapShader)
	{
		m_BumpMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
		//return ViewTransform();
	}
	else if (toonShader) {
		toonShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture(), light->GetLight());
		//return ViewTransform();
	}

	light->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture());
	
	return ViewTransform();
}

bool MODEL::RenderShadow(RNDMATRIXS& renderMatrix, ID3D11ShaderResourceView* texture, LIGHT * _light)
{
	SetTransformMatrix(&renderMatrix);
	D3DXMatrixTranslation(&renderMatrix.world, position.x, position.y, position.z);

	file->Render();
	m_ShadowShader->Render(file->GetIndexCount(), renderMatrix, file->GetTexture(),
		texture, _light->GetPosition(), light->GetLight());
	return ViewTransform();
}

bool MODEL::RenderDepth(RNDMATRIXS& renderMatrix)
{
	D3DXMatrixTranslation(&renderMatrix.world, position.x, position.y, position.z);
	file->Render();
	return m_DepthShader->Render(file->GetIndexCount(), renderMatrix);
}

void MODEL::Shutdown()
{
	if (m_ShadowShader)
	{
		m_ShadowShader->Shutdown();
		SAFE_DELETE(m_ShadowShader);
	}

	if (m_DepthShader)
	{
		m_DepthShader->Shutdown();
		SAFE_DELETE(m_DepthShader);
	}

	if (m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		SAFE_DELETE(m_SpecMapShader);
	}

	if (m_BumpMapShader)
	{
		m_BumpMapShader->Shutdown();
		SAFE_DELETE(m_BumpMapShader);
	}
	
	if (toonShader)
	{
		toonShader->Shutdown();
		SAFE_DELETE(toonShader);
	}

	SAFE_DELETE(material);

	light->Shutdown();
	SAFE_DELETE(light);

	file->Shutdown();
	return;
}

LightBufferType * MODEL::GetLight() { return this->light->GetLight(); }
