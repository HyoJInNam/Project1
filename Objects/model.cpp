
#include "objectstdafx.h"
#include "light.h"
#include "model.h"



MODEL::MODEL(const char* name) : Transform(name)
{
	file = new LOADOBJECTSFILE(device, deviceContext);
	isActive = false;
	rmode = Toon;
}


MODEL::MODEL(const MODEL& other) : Transform(other) {}
MODEL::~MODEL() {}

bool MODEL::ViewTransform()
{
	if (show_inspector)
	{
		ImGui::Begin(GetObjectName(), &show_inspector);
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Close Console")) {
					show_inspector = false;
				}
				ImGui::EndPopup();
			}

			ImGui::Text("transform");
			D3DXVECTOR3 pos = GetPosition();
			ImGui::DragFloat3("position", (float*)&pos, 0.1f, 0, 0);
			SetPosition(pos);

			D3DXVECTOR3 rot = GetRotation();
			ImGui::InputFloat3("rotation", (float*)&rot);
			SetRotation(rot);

			D3DXVECTOR3 scl = GetScale();
			ImGui::InputFloat3("scale", (float*)&scl);
			SetScale(scl);

			ImGui::Text(" ");
			ImGui::Checkbox("Is Spin", &isSpin);
			ImGui::Text(" ");


			ImGui::Text("Render Mode");
			if (ImGui::RadioButton("toon", rmode == Toon)) rmode = Toon;
			if (ImGui::RadioButton("bump", rmode == Bump)) rmode = Bump;
			if (ImGui::RadioButton("bump + toon", rmode == ToonBump)) rmode = ToonBump;
			if (ImGui::RadioButton("ink", rmode == Ink)) rmode = Ink;
			if (ImGui::RadioButton("bump + ink", rmode == SpecBump)) rmode = SpecBump;

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
		{
			m_SpecMapShader = new SpecMapShaderClass(hwnd, device, deviceContext);
			ISINSTANCE(m_SpecMapShader);
			if (!m_SpecMapShader->Initialize())
			{
				MessageBox(hwnd, L"Could not initialize the ink shader object.", L"Error", MB_OK);
				return false;
			}
		}
		{
			m_InkShader = new SpecMapShaderClass(hwnd, device, deviceContext);
			ISINSTANCE(m_InkShader);
			if (!m_InkShader->InkInitialize())
			{
				MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
				return false;
			}
		}

	}
	if (normal_texture_name)
	{
		{
			m_BumpMapShader = new BumpMapShaderClass(hwnd, device, deviceContext);
			ISINSTANCE(m_BumpMapShader);
			if (!m_BumpMapShader->Initialize())
			{
				ERR_MESSAGE(L"Could not initialize the Bump Map shader object.", L"ERROR");
				return false;
			}
		}
		{
			m_ToonShader = new BumpMapShaderClass(hwnd, device, deviceContext);
			ISINSTANCE(m_ToonShader);
			if (!m_ToonShader->ToonInitialize())
			{
				ERR_MESSAGE(L"Could not initialize the toon shader object.", L"ERROR");
				return false;
			}
		}
	}
	//else
	{//if (IsInk == true)
		{
			ToonColorShader = new LIGHTSHADER(hwnd, device, deviceContext);
			ISINSTANCE(ToonColorShader);
			if (!ToonColorShader->ToonInitialize())
			{
				ERR_MESSAGE(L"Could not initialize the toon shader object.", L"ERROR");
				return false;
			}
		}
	}

	//m_DepthShader = new DepthShaderClass(hwnd, device, deviceContext);
	//ISINSTANCE(m_DepthShader);
	//if (!m_DepthShader->Initialize())
	//{
	//	ERR_MESSAGE(L"Could not initialize the depth shader object.", L"ERROR");
	//	return false;
	//}


	//m_ShadowShader = new ShadowShaderClass(hwnd, device, deviceContext);
	//ISINSTANCE(m_ShadowShader);
	//if (!m_ShadowShader->Initialize())
	//{
	//	ERR_MESSAGE(L"Could not initialize the  shadow shader object.", L"ERROR");
	//	return false;
	//}

	
	
	return true;
}

bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos, LIGHT* mainLight)
{
	SetTransformMatrix(&renderMatrix);
	file->Render();
	light->SetDirection(mainLight->GetDirection());


	if (rmode == Ink) m_InkShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
	else if (rmode == SpecBump) m_SpecMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
	else if (rmode == ToonBump) m_ToonShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
	else if (rmode == Bump) m_BumpMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), light->GetLight());
	else if (rmode == Toon)ToonColorShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture(), light->GetLight());
	else light->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture());
	
	return true; // ViewTransform();
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

	if (m_InkShader)
	{
		m_InkShader->Shutdown();
		SAFE_DELETE(m_InkShader);
	}
	
	if (m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		SAFE_DELETE(m_SpecMapShader);
	}

	if (m_ToonShader)
	{
		m_ToonShader->Shutdown();
		SAFE_DELETE(m_ToonShader);
	}

	if (m_BumpMapShader)
	{
		m_BumpMapShader->Shutdown();
		SAFE_DELETE(m_BumpMapShader);
	}
	
	if (ToonColorShader)
	{
		ToonColorShader->Shutdown();
		SAFE_DELETE(ToonColorShader);
	}

	SAFE_DELETE(material);

	light->Shutdown();
	SAFE_DELETE(light);

	file->Shutdown();
	return;
}

LightBufferType * MODEL::GetLight() { return this->light->GetLight(); }
