#pragma once
class INPUTS : public SINGLETON <INPUTS>
{
public:
	INPUTS();
	~INPUTS();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();
};

