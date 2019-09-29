#pragma once

class SYSTEM : public SINGLETON <SYSTEM>
{
public:
	SYSTEM();
	~SYSTEM();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();
};

