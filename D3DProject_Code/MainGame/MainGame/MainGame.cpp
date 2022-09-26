#pragma comment(lib,"winmm")
#include <windows.h>
#include <digitalv.h>

#include "framework.h"
#include "pch.h"


/////////////게임 관련 변수들
hsEngine* Engine;
GameManager* GM;
////////////////////


HWND hWnd;
// 윈도 프로시저의 전방선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void RecalcWindowSize();

// 메인 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
	//srand(GetTickCount64());
	/// 윈도를 등록한다.
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_PROJECT1);
	wcex.lpszClassName = L"PROJECT1";
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	int iMonitorWidth = GetSystemMetrics(SM_CXSCREEN);    // 모니터 가로 해상도
	int iMonitorHeight = GetSystemMetrics(SM_CYSCREEN);    // 모니터 세로 해상도


	



	// 애플리케이션 초기화를 수행합니다:
	hWnd = CreateWindowW(L"PROJECT1", L"My blade seeks vengeance!", WS_OVERLAPPEDWINDOW,
		0, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//엔진 생성
	Engine = new hsEngine();
	Engine->init(hWnd);
	

	//게임 매니저 생성
	GM = new GameManager();
	GM->init(Engine);


	RecalcWindowSize();
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Engine->BeginRender();
			

			//업데이트 먼저
			Engine->Update();
			GM->Update();

			//렌더링
			Engine->Render();
			GM->Render();

			//디버깅
			//Engine->Debug();
			//GM->Debug();
			
			Engine->EndRender();
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		Engine->mClientHeight	= HIWORD(lParam);
		Engine->mClientWidth	= LOWORD(lParam);
		Engine->OnResize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		PAINTSTRUCT pr;
		BeginPaint(hWnd, &pr);
		EndPaint(hWnd, &pr);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void RecalcWindowSize()
{
	
	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	GetWindowRect(hWnd, &nowRect);

	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = 1280;
	newRect.bottom = 720;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}


