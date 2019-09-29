

//#include <windowsx.h>
#include <windows.h>
#include <cassert>

//==============================

#include <string.h>
#include <atlstr.h>
#include <string>

using namespace std;
//using namespace ATL;

//==============================

#include <D3D11.h>
#include <D3DX10math.h>

//==============================


typedef struct WINDOW_DESCRIPTION
{
	HWND Handle;

	BOOL vsync;
	BOOL fullscreen;

	UINT screenWidth;
	UINT screenHeight;
	FLOAT screenDepth;
	FLOAT screenNear;
}WND_DESC, WNDDesc;

typedef struct GEOMETRY_MATRIX
{
	D3DXMATRIX projection;
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX ortho;

}GEOM_MATRIX, GEOMMatrix;


//==============================

#include "./System/singleton.h"
#include "./System/window.h"
#include "./System/d3dclass.h"

//==============================
