#include "../Utility/stdafx.h"
#include "../System/Time.h"
#include "input.h"
#include "graphics.h"
#include "System.h"


SYSTEM::SYSTEM()
{
	Time::GetInstance()->Start();
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
	Time::GetInstance()->Update();
	ISFAIL(INPUTS::GetInstance()->Frame());
	ISFAIL(GRAPHICS::GetInstance()->Frame());
	return GRAPHICS::GetInstance()->Render();
}

void SYSTEM::Shutdown()
{
	GRAPHICS::GetInstance()->Shutdown();
	INPUTS::GetInstance()->Shutdown();
	Time::GetInstance()->destroy();
}
