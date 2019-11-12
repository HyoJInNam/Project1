#pragma once

class InputClass;
class GraphicsClass;
class FpsClass;
class CpuClass;
class TimerClass;


class SYSTEM : public SINGLETON <SYSTEM>
{
public:
	SYSTEM();
	~SYSTEM();

	BOOL Initialize();
	BOOL Frame();
	void Shutdown();

private:
	FpsClass* fps = nullptr;
	CpuClass* m_cpu = nullptr;
	TimerClass* timer = nullptr;
};

