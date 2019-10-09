#pragma once

#include <windows.h>
#include <cassert>

//==============================

#include <atlstr.h>
#include <string.h>
#include <fstream>
#include <string>

using namespace std;

//==============================

#include <dxgi.h>
#include <D3D11.h>
#include <D3DX10math.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D11.lib")

//==============================

#include "./System/singleton.h"
#include "./System/wndDescription.h"
#include "./System/renderTransformation.h"
#include "./System/d3dclass.h"

#define ERR_MESSAGE(m1, m2)			{ MessageBox(WNDDesc::GetInstance()->getHwnd(), m1, m2, MB_OK); }

#define ISINSTANCE(p)				{ if(p == nullptr) { return false; } }
#define ISFAIL(r)					{ if(r == false)   { return false; } }
#define ISFAILED(r)					{ if(FAILED(r))    { return false; } }
#define ISFAILEDFILE(r, f, m1, m2)  { if (FAILED(r)) { if (m1) { this->OutputErrorMessage(f, m1); } else { ERR_MESSAGE(f, m2); } return false;	}}

//==============================

enum RotationDirection { X, Y, Z };
