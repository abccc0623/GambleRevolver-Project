#pragma once

class hsEngine;
class GameManager;
class Keyinput;



class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	static int ScreenSizeX;
	static int ScreenSizeY;
	static hsEngine* ENGINE;
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void RecalcWindowSize();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	//내가 사용할것들
	GameManager* GM;
	Keyinput*	key;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;
