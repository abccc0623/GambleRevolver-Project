#pragma comment(lib,"winmm")
#include <windows.h>
#include "SystemClass.h"
#include "pch.h"

class SystemClass;
int APIENTRY wWinMain
(	
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	hsEngine* ENGINE;
	Keyinput* Key;
	GameManager* GM;



	// System 객체 생성
	SystemClass* System = new SystemClass;
	if (!System)
	{
		return -1;
	}

	//// System 객체 초기화 및 실행
	if (System->Initialize())
	{
		System->Run();
	}
	//
	//// System 객체 종료 및 메모리 반환
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}