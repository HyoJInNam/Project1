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
#include "./System/d3dclass.h"


#define ERR_INSTANCE(p){ if((p) == nullptr){ return false; } }
#define ERR_MESSAGE(m) { MessageBox(WNDDesc::GetInstance()->getHwnd(), m, L"Error", MB_OK); }



