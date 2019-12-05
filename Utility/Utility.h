#pragma once


#include <cassert>
#include <atlstr.h>




#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

#define ERR_MESSAGE(m1, m2)			{ MessageBox(WNDDesc::GetInstance()->getHwnd(), m1, m2, MB_OK); }

#define ISINSTANCE(p)				{ if(p == nullptr) { return false; } }
#define ISFAIL(r)					{ if(r == false)   { return false; } }
#define ISFAILED(r)					{ if(FAILED(r))    { return false; } }
#define ISFAILEDFILE(r, f, m1, m2)  { if (FAILED(r)) { if (m1) { this->OutputErrorMessage(f, m1); } else { ERR_MESSAGE(f, m2); } return false;	}}




//==============================



#include "AlignedAllocationPolicy.h"
#include "singleton.h"
#include "d3dstdafx.h"

#include "d3dclass.h"
#include "rendertextureclass.h"

#include "wndDescription.h"
#include "objectDescription.h"
#include "renderTransformation.h"
