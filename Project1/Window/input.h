#pragma once

class Mouse;
class Keyboard;

class INPUTS : public SINGLETON <INPUTS>
{
public:
	INPUTS();
	~INPUTS();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();
};
