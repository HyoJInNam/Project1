#pragma once

#include <Utility.h>
#include <fstream>
using namespace std;

class LOADFILE
{


protected:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount;
	int indexCount;



protected:
					LOADFILE(ID3D11Device*, ID3D11DeviceContext*);
					LOADFILE(const LOADFILE*) {};
	virtual			~LOADFILE() {}

	virtual bool	InitializeBuffers()	 = 0;
	virtual void	RenderBuffers()		 = 0;

public:
			void	ShutdownBuffers();
			int		GetIndexCount() { return indexCount; }





protected:
	ID3D11ShaderResourceView* m_textures[3] = { nullptr, };
	


protected:
	bool InitializeTextureArray(ID3D11Device*device,
		WCHAR* texture = nullptr,
		WCHAR* normalmap = nullptr,
		WCHAR* specularmap = nullptr);
	
	void ShutdownTextureArray();



public:
	bool LoadTexture(ID3D11Device* device,
		WCHAR* filename1 = nullptr,
		WCHAR* normalmap = nullptr,
		WCHAR* specularmap = nullptr);
	
	void							ReleaseTexture()		{ ShutdownTextureArray(); }
	   
	ID3D11ShaderResourceView**		GetTextures()			{ return m_textures; }
	ID3D11ShaderResourceView*	    GetTexture()			{ return m_textures[0]; }
};

