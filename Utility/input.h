#pragma once

#include "Mouse.h"
#include "Keyboard.h"

class INPUTS : public SINGLETON <INPUTS>
{
public:
	INPUTS();
	~INPUTS();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();
};
