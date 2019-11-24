#pragma once

#include <windows.h>
#include <cassert>

#include <memory.h>
#include <string.h>
#include <string>

using namespace std;

//==============================

#include <Utility.h>
#include <input.h>
#include <timeclass.h>
#pragma comment (lib, "Utility.lib")

#include <camera.h>
#include <cameraControler.h>
#include <cameraPanelControler.h>
#pragma comment (lib, "CameraContol.lib")

//-----------------------------------------------

#include <LoadFile.h>
#include <LoadImgFile.h>
#include <loadObjFile.h>
#include <TextureArrayClass.h>
#pragma comment (lib, "LoadFile.lib")

#include <lightshader.h>
#include <colorshader.h>
#include <textureshader.h>
#include <BumpMapShaderClass.h>
#include <SpecMapShaderClass.h>
#pragma comment (lib, "Shader.lib")

#include <lightclass.h>
#include <BitmapClass.h>
#include <BumpMapping.h>
#pragma comment (lib, "Objects.lib")

