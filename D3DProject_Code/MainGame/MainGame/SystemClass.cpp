#include "pch.h"
#include "SystemClass.h"

int SystemClass::ScreenSizeX = 0;
int SystemClass::ScreenSizeY = 0;

hsEngine* SystemClass::ENGINE = nullptr;
SystemClass::SystemClass()
{
	GM		= nullptr;
	
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	// ������ â ����, ���� ���� ���� �ʱ�ȭ
	int screenWidth = 0;
	int screenHeight = 0;
	
	// ������ ���� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	//Ű��ǲ Ŭ���� 
	key = new Keyinput();
	key->init(m_hinstance, m_hwnd, screenWidth, screenHeight);
	

	//D3D���� Ŭ����
	ENGINE = new hsEngine();
	ENGINE->init(m_hwnd, 1280, 720);

	if (ENGINE == nullptr)
	{
		return false;
	}

	//���� �Ŵ��� Ŭ����
	GM = new GameManager();
	GM->init(ENGINE);
	

	return true;
}


void SystemClass::Shutdown()
{
	delete ENGINE;
	delete GM;


	// Window ���� ó��
	ShutdownWindows();
}


void SystemClass::Run()
{
	// �޽��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ����ڷκ��� ���� �޽����� ���������� �޽��������� ���ϴ�
	while (true)
	{
		// ������ �޽����� ó���մϴ�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���� �޽����� ���� ��� �޽��� ������ Ż���մϴ�
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó���մϴ�.
			if (!Frame())
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				break;
			}
		}

		// ����ڰ� ESCŰ�� �������� Ȯ�� �� ���� ó����
		if (key->IsEscapePressed() == true)
		{
			break;
		}
	}
}


bool SystemClass::Frame()
{
	int mouseX = 0, mouseY = 0;

	
	//������ ����
	ENGINE->BeginRender();

	//����������Ʈ
	ENGINE->Update();
	//���� �Ŵ��� ������Ʈ
	GM->Update();

	//���� ������
	ENGINE->Render();
	//���� �Ŵ��� ������
	GM->Render();

	
	//������ ����
	ENGINE->EndRender();


	//���� �Ұ� ã��
	GM->Delete();
	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	// �ܺ� �����͸� �� ��ü�� �����մϴ�
	ApplicationHandle = this;

	// �� ���α׷��� �ν��Ͻ��� �����ɴϴ�
	m_hinstance = GetModuleHandle(NULL);

	// ���α׷� �̸��� �����մϴ�
	m_applicationName = L"hsGame";

	// windows Ŭ������ �Ʒ��� ���� �����մϴ�.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// windows class�� ����մϴ�
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� �о�ɴϴ�
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 1280;
	int posY = 720;

	// FULL_SCREEN ���� ���� ���� ȭ���� �����մϴ�.
	//if (FULL_SCREEN)
	//{
	//	// Ǯ��ũ�� ���� �����ߴٸ� ����� ȭ�� �ػ󵵸� ����ũ�� �ػ󵵷� �����ϰ� ������ 32bit�� �����մϴ�.
	//	DEVMODE dmScreenSettings;
	//	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	//	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	//	dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
	//	dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
	//	dmScreenSettings.dmBitsPerPel = 32;
	//	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	//
	//	// Ǯ��ũ������ ���÷��� ������ �����մϴ�.
	//	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	//}
	
	// ������ ����� ��� 800 * 600 ũ�⸦ �����մϴ�.
	//screenWidth = ScreenSizeX;
	//screenHeight = ScreenSizeY;

	// ������ â�� ����, ������ �� ��� ������ �մϴ�.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;


	// �����츦 �����ϰ� �ڵ��� ���մϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �����մϴ�
	RecalcWindowSize();
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}


void SystemClass::ShutdownWindows()
{
	// Ǯ��ũ�� ��忴�ٸ� ���÷��� ������ �ʱ�ȭ�մϴ�.
	//if (FULL_SCREEN)
	//{
	//	ChangeDisplaySettings(NULL, 0);
	//}
	


	// â�� �����մϴ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷� �ν��Ͻ��� �����մϴ�
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ܺ������� ������ �ʱ�ȭ�մϴ�
	ApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_SIZE: 
		{
			SystemClass::ScreenSizeX = LOWORD(lparam); 
			SystemClass::ScreenSizeY = HIWORD(lparam);
			if (SystemClass::ENGINE != nullptr)
			{
				SystemClass::ENGINE->OnResize(SystemClass::ScreenSizeX, SystemClass::ScreenSizeY);
			}
			break;
		}
		

		// ������ ���Ḧ Ȯ���մϴ�
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	// �����찡 �������� Ȯ���մϴ�
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	// �� ���� ��� �޽������� �ý��� Ŭ������ �޽��� ó���� �ѱ�ϴ�.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void  SystemClass::RecalcWindowSize()
{

	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(m_hwnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(m_hwnd, GWL_EXSTYLE);

	GetWindowRect(m_hwnd, &nowRect);

	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = 1280;
	newRect.bottom = 720;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// Ŭ���̾�Ʈ �������� ���� ũ��� �� Ŀ�� �Ѵ�. (�ܰ���, Ÿ��Ʋ ��)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(m_hwnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}