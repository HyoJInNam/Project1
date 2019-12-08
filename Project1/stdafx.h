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


#include <imgui.h>
#include <imgui_windx.h>
#pragma comment (lib, "ImGui.lib")



#include <LoadFile.h>
#include <LoadImgFile.h>
#include <loadObjFile.h>
#include <LoadDomeFile.h>
#include <TextureArrayClass.h>
#pragma comment (lib, "LoadFile.lib")


#include <transform.h>
#include <image_bitmap.h>
#include <model.h>
#include <dome.h>
#include <light.h>
#include <camera.h>
#include <cameraControler.h>
#include <cameraPanelControler.h>
#pragma comment (lib, "Objects.lib")


#include <shader.h>
#include <lightshader.h>
#include <ShadowShaderClass.h>
#include <rendertextureclass.h>
#include <depthshaderclass.h>
#pragma comment (lib, "Shader.lib")

