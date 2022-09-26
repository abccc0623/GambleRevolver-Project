#include "EnginePCH.h"
#include "TimerClass.h"


TimerClass::TimerClass()
{
	FpsTime = 0;
	m_count = 0;
}


TimerClass::~TimerClass()
{
}


bool TimerClass::Initialize()
{
	// �� �ý����� ���� Ÿ�̸Ӹ� �����ϴ��� Ȯ���Ͻʽÿ�.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	// �� ī���Ͱ� �� �и� �ʸ��� ƽ�ϴ� Ƚ���� Ȯ���մϴ�.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}


void TimerClass::Frame()
{
	INT64 currentTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	float timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;


	FpsTime += m_frameTime * 0.001f;
	if (FpsTime >= 1.0f) 
	{
		FpsTime -= 1.0f;
		m_fps = m_count;
		m_count = 0;
	}
	else
	{
		m_count++;
	}


	m_startTime = currentTime;

}

float TimerClass::GetTime()
{
	return  m_frameTime * 0.001f;
}

int TimerClass::GetFPS()
{
	return m_fps;
}

