#include "../Utility/stdafx.h"

#include "input.h"
#include "graphics.h"
#include "System.h"


SYSTEM::SYSTEM()
{
	INPUTS::GetInstance()->Initialize();
	GRAPHICS::GetInstance()->Initialize();
}


SYSTEM::~SYSTEM()
{
}

BOOL SYSTEM::Initialize()
{
	return true;
}

BOOL SYSTEM::Frame()
{
	ISFAIL(GRAPHICS::GetInstance()->Frame());
	return GRAPHICS::GetInstance()->Render();
}

void SYSTEM::Shutdown()
{
	GRAPHICS::GetInstance()->Shutdown();
	INPUTS::GetInstance()->Shutdown();
}
