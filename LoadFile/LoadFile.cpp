#include "LoadFile.h"


LOADFILE::LOADFILE(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:vertexBuffer(nullptr), indexBuffer(nullptr)
	, vertexCount(0), indexCount(0)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

