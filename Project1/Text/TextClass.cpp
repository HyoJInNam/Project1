#include "../Utility/stdafx.h"
#include "../Utility/objectDescription.h"

#include "FontClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"


TextClass::TextClass()
{
	hwnd = WNDDesc::GetInstance()->getHwnd();
	device = D3D::GetInstance()->GetDevice();
	deviceContext = D3D::GetInstance()->GetDeviceContext();

	m_screenWidth = WNDDesc::GetInstance()->getSceneWidth();
	m_screenHeight = WNDDesc::GetInstance()->getSceneHeight();
}

TextClass::TextClass(const TextClass& other) {}
TextClass::~TextClass() {}

bool TextClass::Initialize(D3DXMATRIX baseViewMatrix)
{
	bool result;
	m_baseViewMatrix = baseViewMatrix;

	m_Font = new FontClass;
	ISINSTANCE(m_Font);
	if (!m_Font->Initialize(device, const_cast<char*>("./data/fontdata.txt"), const_cast<WCHAR*>(L"./data/font.dds")))
	{
		ERR_MESSAGE(L"Could not initialize the font object.", L"Error");
		return false;
	}
	
	m_FontShader = new FontShaderClass;
	ISINSTANCE(m_FontShader);
	if (!m_FontShader->Initialize(device, hwnd))
	{
		ERR_MESSAGE(L"Could not initialize the font shader object.", L"Error");
		return false;
	}

	//==========================================================================================

	result = InitializeSentence(&m_sentence1, 16);
	ISFAIL(result);

	result = UpdateSentence(m_sentence1, const_cast<char*>("Fps: "), 20, 20, 0.0f, 1.0f, 0.0f);
	ISFAIL(result);

	result = InitializeSentence(&m_sentence2, 16);
	ISFAIL(result);

	result = UpdateSentence(m_sentence2, const_cast<char*>("Cpu: "), 20, 40, 0.0f, 1.0f, 0.0f);
	ISFAIL(result);

	return true;
}


void TextClass::Shutdown()
{
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);

	m_FontShader->Shutdown();
	SAFE_DELETE(m_FontShader);

	m_Font->Shutdown();
	SAFE_DELETE(m_Font);
}


bool TextClass::Render(D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	ISFAIL(RenderSentence(m_sentence1, worldMatrix, orthoMatrix));
	ISFAIL(RenderSentence(m_sentence2, worldMatrix, orthoMatrix));

	return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength)
{
	// ���ο� ���� ��ü�� ����ϴ�.
	*sentence = new SentenceType;
	ISINSTANCE(*sentence);

	// ���� ���۸� null�� �ʱ�ȭ�մϴ�.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// ������ �ִ� ���̸� �����մϴ�.
	(*sentence)->maxLength = maxLength;

	// ���� �迭�� ���� ���� �����մϴ�.
	(*sentence)->vertexCount = 6 * maxLength;

	// �ε��� �迭�� �ε��� ���� �����մϴ�.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// ���� �迭�� ����ϴ�.
	TextVertexType* vertices = new TextVertexType[(*sentence)->vertexCount];
	ISINSTANCE(vertices);

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[(*sentence)->indexCount];
	ISINSTANCE(indices);

	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(vertices, 0, (sizeof(TextVertexType) * (*sentence)->vertexCount));

	// �ε��� �迭�� �ʱ�ȭ�մϴ�.
	for (int i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� �� ���� ������ ������ �����Ѵ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���۸� �����.
	ISFAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer));

	// ���� �ε��� ������ ������ �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer)))
	{
		return false;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue)
{
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	if ((int)strlen(text) > sentence->maxLength)
	{
		return false;
	}

	TextVertexType* vertices = new TextVertexType[sentence->vertexCount];
	ISINSTANCE(vertices);
	memset(vertices, 0, (sizeof(TextVertexType) * sentence->vertexCount));

	//float drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	//float drawY = (float)((m_screenHeight / 2) - positionY);
	
	float drawX = (float)(positionX);
	float drawY = (float)(positionY);

	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ISFAILED(deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	TextVertexType* verticesPtr = (TextVertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(TextVertexType) * sentence->vertexCount));
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	SAFE_DELETE_ARRAY(vertices);

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	SAFE_RELEASE((*sentence)->vertexBuffer);
	SAFE_RELEASE((*sentence)->indexBuffer);
	SAFE_DELETE(sentence);
}


bool TextClass::RenderSentence(SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride = sizeof(TextVertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXVECTOR4 pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	if (!m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor))
	{
		false;
	}

	return true;
}

bool TextClass::SetFps(int fps)
{
	// fps�� 10,000 ���Ϸ� �ڸ��ϴ�.
	if(fps > 9999)
	{
		fps = 9999;
	}

	// fps ������ ���ڿ� �������� ��ȯ�մϴ�.
	char tempString[16] = { 0, };
	_itoa_s(fps, tempString, 10);

	// fps ���ڿ��� �����մϴ�.
	char fpsString[16] = { 0, };
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	float red = 0;
	float green = 0;
	float blue = 0;

	// fps�� 60 �̻��̸� fps ������ ������� �����մϴ�.
	if(fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// fps�� 60���� ���� ��� fps ������ ��������� �����մϴ�.
	if(fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// fps�� 30 �̸��̸� fps ������ ���������� �����մϴ�.
	if(fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// ���� ���� ���۸� �� ���ڿ� ������ ������Ʈ�մϴ�.
	return UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue);
}


bool TextClass::SetCpu(int cpu)
{
	// cpu ������ ���ڿ� �������� ��ȯ�մϴ�.
	char tempString[16] = { 0, };
	_itoa_s(cpu, tempString, 10);

	// cpu ���ڿ��� �����մϴ�.
	char cpuString[16] = { 0, };
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// ���� ���� ���۸� �� ���ڿ� ������ ������Ʈ�մϴ�.
	return UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f);
}