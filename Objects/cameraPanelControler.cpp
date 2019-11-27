#include "camera.h"
#include "cameraPanelControler.h"

CameraPanelControler::CameraPanelControler(const char* name) : CAMERA(name)
{
}

CameraPanelControler::CameraPanelControler(const CameraPanelControler & other) : CAMERA(other)
{
}

CameraPanelControler::~CameraPanelControler()
{
}

void CameraPanelControler::Update()
{
}

void CameraPanelControler::Rotate()
{
}

void CameraPanelControler::Translate()
{
}
