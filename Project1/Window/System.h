#pragma once

class InputClass;
class GraphicsClass;


class SYSTEM : public SINGLETON <SYSTEM>
{
public:
	SYSTEM();
	~SYSTEM();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();

private:
};

