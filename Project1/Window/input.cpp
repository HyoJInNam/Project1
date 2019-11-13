#include "../Utility/stdafx.h"

#include "../inputController/Mouse.h"
#include "../inputController/Keyboard.h"

#include "input.h"



INPUTS::INPUTS() {}
INPUTS::~INPUTS() {}

BOOL INPUTS::Initialize()
{
	return 0;
}

BOOL INPUTS::Frame()
{
	Mouse::GetInstance()->Update();
	Keyboard::GetInstance()->Update();
	return true;
}

void INPUTS::Shutdown()
{
}
