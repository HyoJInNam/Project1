#pragma once

#include <Utility.h>
#include <fstream>
using namespace std;

class LOADFILE
{
public:
	LOADFILE(ID3D11Device*, ID3D11DeviceContext*);
	LOADFILE(const LOADFILE*) {};
	virtual ~LOADFILE() {}

	virtual bool InitializeBuffers() { return true;  };
	virtual void RenderBuffers() {};
	void ShutdownBuffers()
	{
		SAFE_RELEASE(indexBuffer);
		SAFE_RELEASE(vertexBuffer);
		return;
	}

	bool LoadTexture(WCHAR * filename)
	{
		ISFAILED(D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL));
		return true;
	}
	void ReleaseTexture()
	{
		if (!texture) return;
		SAFE_RELEASE(texture);
		return;
	}
	
	ID3D11ShaderResourceView* GetTexture() { return texture; }
	int GetIndexCount() { return indexCount; }


protected:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11ShaderResourceView* texture;

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};

