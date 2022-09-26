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
	// 이 시스템이 고성능 타이머를 지원하는지 확인하십시오.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	// 빈도 카운터가 매 밀리 초마다 틱하는 횟수를 확인합니다.
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

