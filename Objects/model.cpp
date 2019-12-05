
#include "objectstdafx.h"
#include "light.h"
#include "model.h"



MODEL::MODEL(const char* name)
	: Transform(name)
	, shader(nullptr)
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();

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
			D3DXVECTOR4 ambient = obj_light->GetAmbientColor();
			ImGui::DragFloat4("ambient", (float*)&ambient, 0.1f, 0, 0);
			obj_light->SetAmbientColor(ambient);

			D3DXVECTOR4 diffuse = obj_light->GetDiffuseColor();
			ImGui::DragFloat4("diffuse", (float*)&diffuse, 0.1f, 0, 0);
			obj_light->SetDiffuseColor(diffuse);

			D3DXVECTOR4 specular = obj_light->GetSpecularColor();
			ImGui::DragFloat4("specular", (float*)&specular, 0.1f, 0, 0);
			obj_light->SetSpecularColor(specular);

			float specularPower = obj_light->GetSpecularPower();
			ImGui::DragFloat("specular power", (float*)&specularPower, 0.1f, 0, 0);
			obj_light->SetSpecularPower(specularPower);
		}
		ImGui::End();
	}
	return true;
}

bool MODEL::Initialize(char* modelFilename, WCHAR* texture_file_name, WCHAR* normal_texture_name, WCHAR* specularmap)
{
	ISFAIL(file->Initialize(modelFilename));
	file->LoadTexture(device, texture_file_name, normal_texture_name, specularmap);

	obj_light = new LIGHT("obj shader");
	ISINSTANCE(obj_light);
	obj_light->SetDirectionLight();

	if (specularmap)
	{
		m_SpecMapShader = new SpecMapShaderClass(hwnd, device, deviceContext);
		ISINSTANCE(m_SpecMapShader);
		if (!m_SpecMapShader->Initialize())
		{
			MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
			return false;
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


	shader = new LIGHTSHADER(hwnd, device, deviceContext);
	if (!shader->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the light shader object.", L"ERROR");
		return false;
	}
	
	return true;
}

bool MODEL::Render(RNDMATRIXS& renderMatrix, D3DXVECTOR3 cameraPos, LIGHT* light)
{
	SetTransformMatrix(&renderMatrix);
	file->Render();
	obj_light->SetDirection(light->GetDirection());


	if (m_SpecMapShader)
	{
		m_SpecMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), obj_light->GetDirection(),
			obj_light->GetLight());
		return ViewTransform();
	}

	if (m_BumpMapShader)
	{
		m_BumpMapShader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTextures(), 
			obj_light->GetDirection(), obj_light->GetLight());
		return ViewTransform();
	}

	if (shader->Render(file->GetIndexCount(), renderMatrix, cameraPos, file->GetTexture(), obj_light->GetLight()) == false)
	{
		material = new COLORSHADER(hwnd, device, deviceContext);
		ISINSTANCE(material);
		if (!material->Initialize())
		{
			ERR_MESSAGE(L"Could not initialize the texture shader object.", L"ERROR");
		}

		material->Render(file->GetIndexCount(), renderMatrix);
	}


	return ViewTransform();
}

bool MODEL::RenderShadow(RNDMATRIXS& renderMatrix, ID3D11ShaderResourceView* shaderResourceView, LIGHT * light)
{
	D3DXMatrixTranslation(&renderMatrix.world, position.x, position.y, position.z);
	file->Render();
	m_ShadowShader->Render(file->GetIndexCount(), renderMatrix, file->GetTexture(), shaderResourceView, light->GetPosition(), obj_light->GetLight());

	return ViewTransform();
}

bool MODEL::RenderDepth(RNDMATRIXS& renderMatrix)
{
	D3DXMatrixTranslation(&renderMatrix.world, position.x, position.y, position.z);
	file->Render();
	return m_DepthShader->Render(file->GetIndexCount(), renderMatrix);
	//return ViewTransform();
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

	SAFE_DELETE(shader);
	SAFE_DELETE(obj_light);
	SAFE_DELETE(material);

	file->Shutdown();
	return;
}

LightBufferType * MODEL::GetLight() { return this->obj_light->GetLight(); }
