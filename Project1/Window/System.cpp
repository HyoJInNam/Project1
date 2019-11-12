#include "../Utility/stdafx.h"

#include "../System/CpuClass.h"
#include "../System/FpsClass.h"
#include "../System/TimerClass.h"

#include "input.h"
#include "graphics.h"
#include "System.h"


SYSTEM::SYSTEM()
	:fps(nullptr),m_cpu(nullptr), timer(nullptr)
{
	INPUTS::GetInstance()->Initialize();
	GRAPHICS::GetInstance()->Initialize();
}


SYSTEM::~SYSTEM()
{
}

BOOL SYSTEM::Initialize()
{
	fps = new FpsClass;
	ISINSTANCE(fps);
	fps->Initialize();

	m_cpu = new CpuClass;
	ISINSTANCE(m_cpu);
	m_cpu->Initialize();

	timer = new TimerClass;
	ISINSTANCE(timer);
	if (!timer->Initialize())
	{
		ERR_MESSAGE(L"Could not initialize the Timer object.", L"Error");
		return false;
	}

	return true;
}

BOOL SYSTEM::Frame()
{
	timer->Frame();
	fps->Frame();
	m_cpu->Frame();

	ISFAIL(GRAPHICS::GetInstance()->Frame(fps->GetFps(), m_cpu->GetCpuPercentage(), timer->GetTime()));

	return GRAPHICS::GetInstance()->Render();
}

void SYSTEM::Shutdown()
{
	SAFE_DELETE(fps);
	SAFE_DELETE(m_cpu);
	SAFE_DELETE(timer);

	GRAPHICS::GetInstance()->Shutdown();
	INPUTS::GetInstance()->Shutdown();
}
