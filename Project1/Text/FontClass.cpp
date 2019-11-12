#include "../Utility/stdafx.h"
#include "../Utility/objectDescription.h"

#include "FontClass.h"

#include <fstream>
using namespace std;

#include "../Utility/DDSTextureLoader.h"

//======================================================
FontClass::FontClass()	: m_texture(nullptr) {}

FontClass::FontClass(const FontClass& other) {}

FontClass::~FontClass(){}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	if (!LoadFontData(fontFilename))
	{
		return false;
	}

	return LoadTexture(device, textureFilename);
}


void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}


bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	m_Font = new FontType[95];
	ISINSTANCE(m_Font);

	
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}



bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	ISFAILED(DirectX::CreateDDSTextureFromFile(device, filename, nullptr, &m_texture));
	//D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	return true;
}


void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	// 입력 정점을 VertexType 구조로 강제 변환합니다.
	VertexType* vertexPtr = (VertexType*)vertices;

	int numLetters = (int)strlen(sentence);

	int index = 0;
	int letter = 0;

	for (int i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// 문자가 공백이면 3 픽셀 위로 이동합니다.
		if (letter == 0) { drawX = drawX + 3.0f; }
		else
		{
			// 쿼드의 첫 번째 삼각형.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // 왼쪽 위
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // 오른쪽 아래
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // 왼쪽 아래
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			// 쿼드의 두 번째 삼각형.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // 왼쪽 위
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // 오른쪽 위
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // 오른쪽 아래
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// 문자의 크기와 한 픽셀 씩 그리기를 위해 x 위치를 업데이트합니다.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}