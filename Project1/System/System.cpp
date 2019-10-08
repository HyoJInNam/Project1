#include "../stdafx.h"
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
	return false;
}

BOOL SYSTEM::Frame()
{
	bool result;

	result = GRAPHICS::GetInstance()->Frame();
	ISFAIL(result);


	return true;
}

void SYSTEM::Shutdown()
{
	GRAPHICS::GetInstance()->Shutdown();
	INPUTS::GetInstance()->Shutdown();
}
