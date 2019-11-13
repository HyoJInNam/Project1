#pragma once

#include <windows.h>
#include <cassert>

#include <memory.h>
//==============================

#include <atlstr.h>
#include <string.h>
#include <fstream>
#include <string>

using namespace std;

//==============================

#include <dxgi.h>
#include <D3D11.h>
#include <D3DX11tex.h>
#include <D3DX10math.h>
#include <d3dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//==============================

#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

#define ERR_MESSAGE(m1, m2)			{ MessageBox(WNDDesc::GetInstance()->getHwnd(), m1, m2, MB_OK); }

#define ISINSTANCE(p)				{ if(p == nullptr) { return false; } }
#define ISFAIL(r)					{ if(r == false)   { return false; } }
#define ISFAILED(r)					{ if(FAILED(r))    { return false; } }
#define ISFAILEDFILE(r, f, m1, m2)  { if (FAILED(r)) { if (m1) { this->OutputErrorMessage(f, m1); } else { ERR_MESSAGE(f, m2); } return false;	}}


//==============================


#include "../Utility/AlignedAllocationPolicy.h"

#include "../Utility/singleton.h"
#include "../Utility/wndDescription.h"
#include "../Utility/objectDescription.h"
#include "../Utility/renderTransformation.h"

#include "../Window/d3dclass.h"


//==============================

enum RotationDirection { X, Y, Z };
